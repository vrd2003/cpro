#include "GeminiClient.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSettings>
#include <QInputDialog>
#include <QLineEdit>

// ── Key management ────────────────────────────────────────────────────────────

QString GeminiClient::loadApiKey() {
    // 1. Persistent storage (set on a previous launch)
    QSettings settings("DSA Tools", "ComplexityAnalyzer");
    QString key = settings.value("gemini/apiKey").toString();
    if (!key.isEmpty()) return key;

    // 2. Environment variable (CI / developer machines)
    key = qgetenv("GEMINI_API_KEY");
    if (!key.isEmpty()) {
        storeApiKey(key);
        return key;
    }

    // 3. One-time prompt — stored immediately so it is never asked again
    bool ok = false;
    key = QInputDialog::getText(
        nullptr,
        "Gemini API Key",
        "Enter your Gemini API key:\n"
        "(Saved in QSettings — you will not be asked again)",
        QLineEdit::Normal,
        QString(), &ok
    );
    if (ok && !key.isEmpty()) {
        storeApiKey(key);
        return key;
    }
    return {};
}

void GeminiClient::storeApiKey(const QString& key) {
    QSettings settings("DSA Tools", "ComplexityAnalyzer");
    settings.setValue("gemini/apiKey", key);
}

void GeminiClient::setApiKey(const QString& key) {
    m_apiKey = key;
    storeApiKey(key);
}

// ── Construction ──────────────────────────────────────────────────────────────

GeminiClient::GeminiClient(QObject* parent)
    : QObject(parent)
    , m_nam(new QNetworkAccessManager(this))
    , m_apiKey(loadApiKey())
{
    connect(m_nam, &QNetworkAccessManager::finished,
            this,  &GeminiClient::onReply);
}

// ── Public API ────────────────────────────────────────────────────────────────

void GeminiClient::sendAnalysis(const QString& code) {
    if (m_apiKey.isEmpty()) {
        emit errorOccurred("No API key configured.");
        return;
    }

    QUrl url(
        "https://generativelanguage.googleapis.com/v1beta/"
        "models/gemini-2.5-flash:generateContent?key=" + m_apiKey
    );
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // ── System instruction ────────────────────────────────────────────────
    QJsonObject sysParts;
    sysParts["text"] =
        "You are an expert C++ code analyzer. Analyze the provided code and "
        "return a JSON object with EXACTLY the following format:\n"
        "{\n"
        "  \"overall_time_complexity\": \"O(...)\",\n"
        "  \"overall_space_complexity\": \"O(...)\",\n"
        "  \"functions\": [\n"
        "    { \"name\": \"functionName\", \"time_complexity\": \"O(...)\", \"space_complexity\": \"O(...)\" }\n"
        "  ],\n"
        "  \"algorithms_used\": [\"Algo1\", \"Algo2\"],\n"
        "  \"suggestions\": [\n"
        "    { \"icon\": \"💡\", \"text\": \"Short suggestion...\", \"severity\": 1 }\n"
        "  ]\n"
        "}\n"
        "If there are no explicit functions, still populate 'functions' for the 'main' body. "
        "Severity is 1 (minor) to 5 (critical).\n"
        "Response must strictly be the raw JSON object.";

    QJsonArray sysPartsArr;
    sysPartsArr.append(sysParts);
    QJsonObject systemInstruction;
    systemInstruction["parts"] = sysPartsArr;

    // ── User message ──────────────────────────────────────────────────────
    QJsonObject userParts;
    userParts["text"] = code;
    QJsonArray userPartsArr;
    userPartsArr.append(userParts);
    QJsonObject userMessage;
    userMessage["role"]  = "user";
    userMessage["parts"] = userPartsArr;

    QJsonArray contents;
    contents.append(userMessage);

    QJsonObject generationConfig;
    generationConfig["responseMimeType"] = "application/json";

    QJsonObject payload;
    payload["contents"]           = contents;
    payload["system_instruction"] = systemInstruction;
    payload["generationConfig"]   = generationConfig;

    m_nam->post(request, QJsonDocument(payload).toJson());
}

// ── Private: response handler ─────────────────────────────────────────────────

void GeminiClient::onReply(QNetworkReply* reply) {
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred(reply->errorString());
        return;
    }

    QString errOut;
    QJsonObject result = parseResponse(reply->readAll(), errOut);
    if (!errOut.isEmpty()) {
        emit errorOccurred(errOut);
        return;
    }
    emit resultReady(result);
}

// ── Private: JSON parsing ─────────────────────────────────────────────────────

QJsonObject GeminiClient::parseResponse(const QByteArray& data, QString& errOut) {
    QJsonDocument outer = QJsonDocument::fromJson(data);
    QJsonObject   root  = outer.object();

    // API-level error
    if (root.contains("error")) {
        errOut = root["error"].toObject()["message"].toString();
        return {};
    }

    QJsonArray candidates = root["candidates"].toArray();
    if (candidates.isEmpty()) {
        errOut = "Empty response from Gemini";
        return {};
    }

    QString text = candidates[0]
        .toObject()["content"]
        .toObject()["parts"]
        .toArray()[0]
        .toObject()["text"]
        .toString()
        .trimmed();

    // Strip optional markdown fences
    if (text.startsWith("```json", Qt::CaseInsensitive)) {
        text.remove(0, 7);
        if (text.endsWith("```")) text.chop(3);
        text = text.trimmed();
    } else if (text.startsWith("```")) {
        text.remove(0, 3);
        if (text.endsWith("```")) text.chop(3);
        text = text.trimmed();
    }

    QJsonParseError parseErr;
    QJsonDocument respDoc = QJsonDocument::fromJson(text.toUtf8(), &parseErr);
    if (parseErr.error != QJsonParseError::NoError) {
        errOut = "JSON parse error: " + parseErr.errorString();
        return {};
    }
    return respDoc.object();
}
