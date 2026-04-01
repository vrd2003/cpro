#include "MainWindow.h"
#include "SyntaxHighlighter.h"
#include "ChartHelper.h"
#include "AppStyles.h"
#include "ExampleSnippets.h"
#include "../src/ComplexityClass.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QFont>
#include <QFontDatabase>
#include <QStatusBar>
#include <QFrame>
#include <QGroupBox>
#include <QApplication>
#include <QPropertyAnimation>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QtCharts/QChartView>
#include <QScrollBar>
#include <QInputDialog>
#include <QLineEdit>
#include <unordered_map>

using namespace std;

// ── Construction ──────────────────────────────────────────────────────────────

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("🔍 C++ DSA Code Analyzer & Visualizer");
    setMinimumSize(1200, 720);
    applyTheme();
    buildUi();
}

void MainWindow::applyTheme() {
    setStyleSheet(k_stylesheet);
}

void MainWindow::buildUi() {
    // ── Left panel ──────────────────────────────────────────────────────────
    auto* leftWidget = new QWidget;
    auto* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setSpacing(8);
    leftLayout->setContentsMargins(8, 8, 8, 8);

    auto* editorLabel = new QLabel("📝  C++ Code Editor");
    editorLabel->setStyleSheet("color:#89b4fa; font-weight:bold; font-size:14px;");
    leftLayout->addWidget(editorLabel);

    m_exampleCombo = new QComboBox;
    for (const auto& [name, _] : k_examples) m_exampleCombo->addItem(name);
    connect(m_exampleCombo, QOverload<int>::of(&QComboBox::activated),
            this, &MainWindow::onLoadExample);
    leftLayout->addWidget(m_exampleCombo);

    m_editor = new QPlainTextEdit;
    m_editor->setPlaceholderText(
        "// Paste or type your C++ DSA code here...\n"
        "// e.g.:\n// for (int i = 0; i < n; i++) {\n"
        "//     for (int j = 0; j < n; j++) { ... }\n// }");
    QFont codeFont("Consolas", 13);
    codeFont.setFixedPitch(true);
    m_editor->setFont(codeFont);
    m_highlighter = new SyntaxHighlighter(m_editor->document());
    leftLayout->addWidget(m_editor, 1);

    auto* btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(8);
    
    m_analyzeBtn = new QPushButton("🔍  Analyze Complexity");
    m_analyzeBtn->setObjectName("analyzeBtn");
    m_analyzeBtn->setCursor(Qt::PointingHandCursor);
    m_analyzeBtn->setMinimumHeight(42);
    connect(m_analyzeBtn, &QPushButton::clicked, this, &MainWindow::onAnalyze);
    
    m_apiKeyBtn = new QPushButton("🔑 Key");
    m_apiKeyBtn->setCursor(Qt::PointingHandCursor);
    m_apiKeyBtn->setMinimumHeight(42);
    m_apiKeyBtn->setToolTip("Change Gemini API Key");
    m_apiKeyBtn->setStyleSheet("background: #313244; color: #cdd6f4; border-radius: 8px; font-weight: bold; font-size: 14px; padding: 0 16px;");
    connect(m_apiKeyBtn, &QPushButton::clicked, this, &MainWindow::onChangeApiKey);

    btnLayout->addWidget(m_analyzeBtn, 4);
    btnLayout->addWidget(m_apiKeyBtn, 1);
    
    leftLayout->addLayout(btnLayout);
    leftWidget->setMinimumWidth(420);

    // ── Right panel ─────────────────────────────────────────────────────────
    auto* rightSplitter = new QSplitter(Qt::Vertical);

    m_topTabs = new QTabWidget;
    
    // AST Tab
    m_astTree = new QTreeWidget;
    m_astTree->setHeaderLabels({"Node", "Depth", "Line"});
    m_astTree->setColumnWidth(0, 260);
    m_astTree->setColumnWidth(1, 60);
    m_astTree->setAlternatingRowColors(true);
    m_astTree->setFrameStyle(QFrame::NoFrame);
    m_topTabs->addTab(m_astTree, "🌳 AST Tree");

    // Functions Tab
    m_funcTable = new QTableWidget(0, 3);
    m_funcTable->setHorizontalHeaderLabels({"Function Name", "Time O()", "Space O()"});
    m_funcTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_funcTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_funcTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_funcTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_funcTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_funcTable->setShowGrid(false);
    m_topTabs->addTab(m_funcTable, "⚡ Function Complexities");

    rightSplitter->addWidget(m_topTabs);

    auto* bottomWidget = new QWidget;
    auto* bottomLayout = new QHBoxLayout(bottomWidget);
    bottomLayout->setSpacing(8);
    bottomLayout->setContentsMargins(4, 8, 4, 4);

    auto* chartGroup  = new QGroupBox("📈  Complexity Growth Chart");
    auto* chartLayout = new QVBoxLayout(chartGroup);
    auto* initChart   = ChartHelper::buildChart(ComplexityClass::O1);
    m_chartView = new QChartView(initChart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setMinimumHeight(260);
    m_chartView->setBackgroundBrush(QColor("#1e1e2e"));
    m_chartView->setFrameStyle(QFrame::NoFrame);
    chartLayout->addWidget(m_chartView);
    bottomLayout->addWidget(chartGroup, 3);

    auto* suggGroup  = new QGroupBox("💡  Optimization Suggestions");
    auto* suggLayout = new QVBoxLayout(suggGroup);
    m_suggList = new QListWidget;
    m_suggList->setWordWrap(true);
    m_suggList->setResizeMode(QListView::Adjust);
    m_suggList->setSpacing(4);
    suggLayout->addWidget(m_suggList);
    bottomLayout->addWidget(suggGroup, 2);

    rightSplitter->addWidget(bottomWidget);
    rightSplitter->setStretchFactor(0, 4);
    rightSplitter->setStretchFactor(1, 3);

    // ── Main splitter ────────────────────────────────────────────────────────
    auto* mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->addWidget(leftWidget);
    mainSplitter->addWidget(rightSplitter);
    mainSplitter->setStretchFactor(0, 2);
    mainSplitter->setStretchFactor(1, 3);
    mainSplitter->setHandleWidth(4);
    setCentralWidget(mainSplitter);

    // ── Status bar ───────────────────────────────────────────────────────────
    m_complexityBadge = new QLabel("  Complexity: —  ");
    m_complexityBadge->setStyleSheet(
        "background:#313244; color:#cba6f7; border-radius:4px;"
        "padding:2px 8px; font-weight:bold; font-size:14px;");
    m_statsLabel = new QLabel("  Ready  ");
    m_statsLabel->setStyleSheet("color:#a6adc8;");
    statusBar()->addPermanentWidget(m_complexityBadge);
    statusBar()->addWidget(m_statsLabel);

    // ── GeminiClient ─────────────────────────────────────────────────────────
    m_gemini = new GeminiClient(this);

    connect(m_gemini, &GeminiClient::resultReady,
            this, [this](const QJsonObject& result) {
                m_analyzeBtn->setEnabled(true);
                applyAnalysisResult(result);
            });

    connect(m_gemini, &GeminiClient::errorOccurred,
            this, [this](const QString& msg) {
                m_analyzeBtn->setEnabled(true);
                m_statsLabel->setText("  API Error: " + msg + "  ");
                m_complexityBadge->setText("  Error  ");
            });
}

// ── Slots ─────────────────────────────────────────────────────────────────────

void MainWindow::onLoadExample(int index) {
    if (index <= 0) return;
    m_editor->setPlainText(k_examples[static_cast<size_t>(index)].second);
    m_exampleCombo->setCurrentIndex(0);
}

void MainWindow::onAnalyze() {
    const QString code = m_editor->toPlainText().trimmed();
    if (code.isEmpty()) {
        m_complexityBadge->setText("  O(1) — empty input  ");
        m_astTree->clear();
        m_suggList->clear();
        return;
    }

    // Local AST analysis (fast, synchronous)
    Analyzer analyzer;
    AnalysisResult res = analyzer.analyze(code.toStdString());
    m_astTree->clear();
    m_funcTable->setRowCount(0);
    if (res.ast) {
        populateAST(res.ast, nullptr);
        m_astTree->expandAll();
    }

    // Remote Gemini analysis (async — disables button until response arrives)
    m_complexityBadge->setText("  Complexity: Loading...  ");
    m_statsLabel->setText("  Analyzing with Gemini...  ");
    m_suggList->clear();
    m_analyzeBtn->setEnabled(false);
    m_gemini->sendAnalysis(code);
}

void MainWindow::onChangeApiKey() {
    bool ok = false;
    QString newKey = QInputDialog::getText(
        this,
        "Change Gemini API Key",
        "Enter new Gemini API key\n(Leave blank to keep current):",
        QLineEdit::Normal,
        QString(), &ok
    );
    if (ok && !newKey.isEmpty()) {
        m_gemini->setApiKey(newKey);
    }
}

// ── Analysis result helpers ───────────────────────────────────────────────────

ComplexityClass MainWindow::classifyComplexity(const QString& tc) const {
    // Evaluated in decreasing order so the highest class wins
    if (tc.contains("2^N", Qt::CaseInsensitive) ||
        tc.contains("exp", Qt::CaseInsensitive) ||
        tc.contains("!",   Qt::CaseSensitive))        return ComplexityClass::ONK;
    if (tc.contains("N^3",  Qt::CaseInsensitive) ||
        tc.contains("N\u00b3", Qt::CaseInsensitive))  return ComplexityClass::ON3;
    if (tc.contains("N^2 log", Qt::CaseInsensitive) ||
        tc.contains("N\u00b2 log", Qt::CaseInsensitive)) return ComplexityClass::ON2LogN;
    if (tc.contains("N^2", Qt::CaseInsensitive) ||
        tc.contains("N\u00b2", Qt::CaseInsensitive) ||
        tc.contains("V^2", Qt::CaseInsensitive) ||
        tc.contains("E^2", Qt::CaseInsensitive))      return ComplexityClass::ON2;
    if (tc.contains("N log", Qt::CaseInsensitive) ||
        tc.contains("V log", Qt::CaseInsensitive) ||
        tc.contains("E log", Qt::CaseInsensitive))    return ComplexityClass::ONLogN;
    if (tc.contains("log",   Qt::CaseInsensitive))    return ComplexityClass::OLogN;
    if (tc.contains("N",     Qt::CaseInsensitive) ||
        tc.contains("V",     Qt::CaseInsensitive) ||
        tc.contains("E",     Qt::CaseInsensitive))    return ComplexityClass::ON;
    return ComplexityClass::O1;
}

void MainWindow::applyAnalysisResult(const QJsonObject& result) {
    // ── Complexity badge ──────────────────────────────────────────────────
    const QString tc = result["overall_time_complexity"].toString();
    m_complexityBadge->setText("  Complexity: " + tc + "  ");

    // ── Status bar: space + algorithms ───────────────────────────────────
    const QString sc = result["overall_space_complexity"].toString();
    QStringList algoList;
    for (const auto& a : result["algorithms_used"].toArray())
        algoList << a.toString();
    QString stats = "  🧠 " + algoList.join(" + ");
    if (!sc.isEmpty()) stats += "  │  Space: " + sc;
    stats += "  ";
    m_statsLabel->setText(stats);

    // ── Functions Table ───────────────────────────────────────────────────
    m_funcTable->setRowCount(0);
    const QJsonArray funcs = result["functions"].toArray();
    for (const auto& fVal : funcs) {
        QJsonObject fObj = fVal.toObject();
        int row = m_funcTable->rowCount();
        m_funcTable->insertRow(row);
        
        m_funcTable->setItem(row, 0, new QTableWidgetItem(fObj["name"].toString()));
        m_funcTable->setItem(row, 1, new QTableWidgetItem(fObj["time_complexity"].toString()));
        m_funcTable->setItem(row, 2, new QTableWidgetItem(fObj["space_complexity"].toString()));
    }
    m_topTabs->setCurrentWidget(m_funcTable); // Auto-focus the functions tab on success

    // ── Suggestions ───────────────────────────────────────────────────────
    m_suggList->clear();
    const QJsonArray suggs = result["suggestions"].toArray();
    if (suggs.isEmpty()) {
        auto* item = new QListWidgetItem("✅  No issues found — code looks efficient!");
        item->setForeground(QColor("#a6e3a1"));
        m_suggList->addItem(item);
    } else {
        for (const auto& s : suggs) {
            const QJsonObject obj = s.toObject();
            const QString icon   = obj["icon"].toString();
            const QString text   = obj["text"].toString();
            const int severity   = obj["severity"].toInt();

            auto* item = new QListWidgetItem(icon + "  " + text);
            item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
            switch (severity) {
                case 5:  item->setForeground(QColor("#f38ba8")); break;
                case 4:  item->setForeground(QColor("#fab387")); break;
                case 3:  item->setForeground(QColor("#f9e2af")); break;
                case 2:  item->setForeground(QColor("#89dceb")); break;
                default: item->setForeground(QColor("#a6e3a1")); break;
            }
            m_suggList->addItem(item);
        }
    }

    // ── Chart ─────────────────────────────────────────────────────────────
    updateChart(classifyComplexity(tc));
}

// ── Helpers ───────────────────────────────────────────────────────────────────

void MainWindow::populateAST(const shared_ptr<ASTNode>& node,
                              QTreeWidgetItem* parentItem) {
    if (!node) return;
    const QString icon      = nodeIcon(node->label);
    const QString nodeLabel = icon + " " + QString::fromStdString(node->label);

    QTreeWidgetItem* item;
    if (!parentItem) {
        item = new QTreeWidgetItem(m_astTree, {nodeLabel, "", ""});
        item->setForeground(0, QColor("#89b4fa"));
    } else {
        item = new QTreeWidgetItem(parentItem,
            {nodeLabel,
             QString::number(node->depth),
             node->lineNumber > 0 ? QString::number(node->lineNumber) : "-"});

        const QString lbl = QString::fromStdString(node->label);
        if (lbl == "for" || lbl == "while" || lbl == "do")
            item->setForeground(0, QColor("#fab387"));
        else if (lbl == "sort()")
            item->setForeground(0, QColor("#cba6f7"));
        else if (lbl.contains("binary") || lbl.contains("lower") || lbl.contains("upper"))
            item->setForeground(0, QColor("#89dceb"));
        else if (lbl == "if" || lbl == "else")
            item->setForeground(0, QColor("#a6e3a1"));
        else
            item->setForeground(0, QColor("#cdd6f4"));
    }

    for (const auto& child : node->children)
        populateAST(child, item);
}

void MainWindow::updateChart(ComplexityClass cls) {
    auto* newChart = ChartHelper::buildChart(cls);
    auto* oldChart = m_chartView->chart();
    m_chartView->setChart(newChart);
    m_chartView->setBackgroundBrush(QColor("#1e1e2e"));
    m_chartView->setFrameStyle(QFrame::NoFrame);
    if (oldChart) delete oldChart;
}

QString MainWindow::nodeIcon(const string& label) const {
    // O(1) lookup via static map — replaces sequential if-chain
    static const unordered_map<string, QString> icons = {
        {"for",      "🔁"},
        {"while",    "🔄"},
        {"do",       "🔂"},
        {"sort()",   "⬇"},
        {"if",       "⑂"},
        {"else",     "↩"},
        {"Program",  "📄"},
    };

    auto it = icons.find(label);
    if (it != icons.end()) return it->second;

    if (label.find("binary") != string::npos ||
        label.find("lower")  != string::npos ||
        label.find("upper")  != string::npos)
        return "🔍";

    return "⚙";
}
