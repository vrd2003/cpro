#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>

// ── GeminiClient ─────────────────────────────────────────────────────────────
// Owns all Gemini API networking. Extracted from MainWindow to satisfy SRP.
//
// Usage:
//   GeminiClient* gc = new GeminiClient(this);
//   connect(gc, &GeminiClient::resultReady,   this, &MyWindow::onResult);
//   connect(gc, &GeminiClient::errorOccurred, this, &MyWindow::onError);
//   gc->sendAnalysis(codeString);

class GeminiClient : public QObject {
    Q_OBJECT

public:
    explicit GeminiClient(QObject* parent = nullptr);

    // Sends code to Gemini for analysis. Emits resultReady or errorOccurred
    // asynchronously when the response arrives.
    void sendAnalysis(const QString& code);

    // Key management — reads from QSettings, falls back to env var
    // GEMINI_API_KEY, then prompts the user once via QInputDialog.
    static QString loadApiKey();
    static void    storeApiKey(const QString& key);
    void           setApiKey(const QString& key);

signals:
    void resultReady(const QJsonObject& result);
    void errorOccurred(const QString& message);

private slots:
    void onReply(QNetworkReply* reply);

private:
    QNetworkAccessManager* m_nam;
    QString                m_apiKey;

    // Strips markdown fences and parses the JSON body from a raw API response.
    // Returns a null QJsonObject and sets errOut on failure.
    static QJsonObject parseResponse(const QByteArray& data, QString& errOut);
};
