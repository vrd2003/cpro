#include "MainWindow.h"
#include "SyntaxHighlighter.h"
#include "ChartHelper.h"
#include "../src/ComplexityClass.h"
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
#include <QtCharts/QChartView>
#include <QScrollBar>

using namespace std;

// ── Example snippets ────────────────────────────────────────────────────────
static const vector<pair<QString, QString>> k_examples = {
    {"-- Load Example --", ""},
    // Sorting
    {"O(1) — Constant access",
        "int getFirst(int arr[], int n) {\n"
        "    return arr[0];\n"
        "}"},
    {"O(n) — Linear search",
        "int linearSearch(int arr[], int n, int x) {\n"
        "    for (int i = 0; i < n; i++) {\n"
        "        if (arr[i] == x) {\n"
        "            return i;\n"
        "        }\n"
        "    }\n"
        "    return -1;\n"
        "}"},
    {"O(log n) — Binary search",
        "#include <vector>\n"
        "#include <algorithm>\n"
        "using namespace std;\n\n"
        "bool found(vector<int>& v, int x) {\n"
        "    sort(v.begin(), v.end());\n"
        "    return binary_search(v.begin(), v.end(), x);\n"
        "}"},
    {"O(n log n) — sort",
        "#include <vector>\n"
        "#include <algorithm>\n"
        "using namespace std;\n\n"
        "void sortArray(vector<int>& v) {\n"
        "    sort(v.begin(), v.end());\n"
        "}"},
    {"O(n²) — Bubble Sort",
        "#include <algorithm>\n"
        "using namespace std;\n\n"
        "void bubbleSort(int arr[], int n) {\n"
        "    for (int i = 0; i < n; i++) {\n"
        "        for (int j = 0; j < n - i - 1; j++) {\n"
        "            if (arr[j] > arr[j+1]) {\n"
        "                swap(arr[j], arr[j+1]);\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}"},
    {"O(n²) — Selection Sort",
        "#include <algorithm>\n"
        "using namespace std;\n\n"
        "void selectionSort(int arr[], int n) {\n"
        "    for (int i = 0; i < n; i++) {\n"
        "        int minIdx = i;\n"
        "        for (int j = i + 1; j < n; j++) {\n"
        "            if (arr[j] < arr[minIdx]) {\n"
        "                minIdx = j;\n"
        "            }\n"
        "        }\n"
        "        swap(arr[i], arr[minIdx]);\n"
        "    }\n"
        "}"},
    {"O(n² log n) — Nested + sort",
        "#include <vector>\n"
        "#include <algorithm>\n"
        "using namespace std;\n\n"
        "void process(vector<int>& v, int n) {\n"
        "    for (int i = 0; i < n; i++) {\n"
        "        for (int j = 0; j < n; j++) {\n"
        "            sort(v.begin(), v.end());\n"
        "        }\n"
        "    }\n"
        "}"},
    {"O(n³) — Triple nested",
        "void tripleLoop(int n) {\n"
        "    for (int i = 0; i < n; i++) {\n"
        "        for (int j = 0; j < n; j++) {\n"
        "            for (int k = 0; k < n; k++) {\n"
        "                // O(n³)\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}"},
    // Graph
    {"BFS — O(V+E)",
        "#include <vector>\n"
        "#include <queue>\n"
        "using namespace std;\n\n"
        "void bfs(int start, vector<vector<int>>& adj, int n) {\n"
        "    vector<bool> visited(n, false);\n"
        "    queue<int> q;\n"
        "    q.push(start);\n"
        "    visited[start] = true;\n"
        "    while (!q.empty()) {\n"
        "        int node = q.front();\n"
        "        q.pop();\n"
        "        for (int nb : adj[node]) {\n"
        "            if (!visited[nb]) {\n"
        "                visited[nb] = true;\n"
        "                q.push(nb);\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}"},
    {"DFS Recursive — O(V+E)",
        "#include <vector>\n"
        "using namespace std;\n\n"
        "void dfs(int node, vector<vector<int>>& adj, vector<bool>& visited) {\n"
        "    visited[node] = true;\n"
        "    for (int nb : adj[node]) {\n"
        "        if (!visited[nb]) {\n"
        "            dfs(nb, adj, visited);\n"
        "        }\n"
        "    }\n"
        "}"},
    // DP
    {"DP — Fibonacci (Tabulation)",
        "#include <vector>\n"
        "using namespace std;\n\n"
        "int fib(int n) {\n"
        "    vector<int> dp(n + 1, 0);\n"
        "    dp[0] = 0;\n"
        "    dp[1] = 1;\n"
        "    for (int i = 2; i <= n; i++) {\n"
        "        dp[i] = dp[i-1] + dp[i-2];\n"
        "    }\n"
        "    return dp[n];\n"
        "}"},
    {"DP — 0/1 Knapsack O(nW)",
        "#include <vector>\n"
        "#include <algorithm>\n"
        "using namespace std;\n\n"
        "int knapsack(int W, int wt[], int val[], int n) {\n"
        "    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));\n"
        "    for (int i = 1; i <= n; i++) {\n"
        "        for (int w = 0; w <= W; w++) {\n"
        "            dp[i][w] = dp[i-1][w];\n"
        "            if (wt[i-1] <= w) {\n"
        "                dp[i][w] = max(dp[i][w], dp[i-1][w-wt[i-1]] + val[i-1]);\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "    return dp[n][W];\n"
        "}"},
    // Hashing / Two-pointer
    {"Hashing — Two Sum O(n)",
        "#include <vector>\n"
        "#include <unordered_map>\n"
        "using namespace std;\n\n"
        "pair<int,int> twoSum(vector<int>& nums, int target) {\n"
        "    unordered_map<int,int> mp;\n"
        "    for (int i = 0; i < (int)nums.size(); i++) {\n"
        "        if (mp.count(target - nums[i])) {\n"
        "            return {mp[target-nums[i]], i};\n"
        "        }\n"
        "        mp[nums[i]] = i;\n"
        "    }\n"
        "    return {-1,-1};\n"
        "}"},
    {"Greedy — Min Heap (Dijkstra style)",
        "#include <vector>\n"
        "#include <queue>\n"
        "using namespace std;\n\n"
        "void dijkstra(int src, vector<pair<int,int>> adj[], int n) {\n"
        "    vector<int> dist(n, INT_MAX);\n"
        "    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;\n"
        "    dist[src] = 0;\n"
        "    pq.push({0, src});\n"
        "    while (!pq.empty()) {\n"
        "        auto [d, u] = pq.top();\n"
        "        pq.pop();\n"
        "        for (auto [w, v] : adj[u]) {\n"
        "            if (dist[u] + w < dist[v]) {\n"
        "                dist[v] = dist[u] + w;\n"
        "                pq.push({dist[v], v});\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}"},
    {"Bit Manipulation — Count set bits",
        "int countBits(int n) {\n"
        "    int count = 0;\n"
        "    while (n > 0) {\n"
        "        count += n & 1;\n"
        "        n >>= 1;\n"
        "    }\n"
        "    return count;\n"
        "}"},
};

// ── Theme stylesheet ─────────────────────────────────────────────────────────
static const char* k_stylesheet = R"(
QMainWindow, QWidget {
    background-color: #1e1e2e;
    color: #cdd6f4;
    font-family: "Segoe UI", sans-serif;
    font-size: 13px;
}
QPlainTextEdit {
    background-color: #181825;
    color: #cdd6f4;
    border: 1px solid #313244;
    border-radius: 6px;
    padding: 6px;
    selection-background-color: #585b70;
    font-family: "Consolas", "Courier New", monospace;
    font-size: 13px;
}
QPlainTextEdit:focus { border: 1px solid #89b4fa; }
QPushButton#analyzeBtn {
    background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                 stop:0 #1e66f5, stop:1 #89b4fa);
    color: #1e1e2e;
    border: none;
    border-radius: 8px;
    padding: 10px 24px;
    font-weight: bold;
    font-size: 14px;
}
QPushButton#analyzeBtn:hover {
    background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                 stop:0 #89b4fa, stop:1 #1e66f5);
}
QPushButton#analyzeBtn:pressed { background: #313244; }
QComboBox {
    background: #313244;
    color: #cdd6f4;
    border: 1px solid #45475a;
    border-radius: 6px;
    padding: 4px 8px;
}
QComboBox::drop-down { border: none; }
QComboBox QAbstractItemView {
    background: #313244;
    color: #cdd6f4;
    selection-background-color: #45475a;
}
QTreeWidget {
    background: #181825;
    color: #cdd6f4;
    border: 1px solid #313244;
    border-radius: 6px;
    alternate-background-color: #1e1e2e;
}
QTreeWidget::item:hover      { background: #313244; }
QTreeWidget::item:selected   { background: #45475a; color: #cdd6f4; }
QTreeWidget QHeaderView::section {
    background: #313244;
    color: #89b4fa;
    border: none;
    padding: 4px;
    font-weight: bold;
}
QListWidget {
    background: #181825;
    color: #cdd6f4;
    border: 1px solid #313244;
    border-radius: 6px;
}
QListWidget::item { padding: 6px; border-bottom: 1px solid #313244; }
QListWidget::item:hover    { background: #313244; }
QListWidget::item:selected { background: #45475a; }
QGroupBox {
    border: 1px solid #313244;
    border-radius: 6px;
    margin-top: 24px;
    padding-top: 8px;
    color: #89b4fa;
    font-weight: bold;
}
QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    left: 10px;
    top: 2px;
    background: transparent;
    padding: 0 4px;
}
QStatusBar { background: #181825; color: #a6adc8; }
QScrollBar:vertical, QScrollBar:horizontal {
    background: #181825;
    width: 8px; height: 8px;
    border: none;
}
QScrollBar::handle:vertical, QScrollBar::handle:horizontal {
    background: #45475a; border-radius: 4px;
}
QScrollBar::add-line, QScrollBar::sub-line { height: 0; width: 0; }
)";

// ── Construction ─────────────────────────────────────────────────────────────
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
    // ── Left panel ──────────────────────────────────────────────────────
    auto* leftWidget = new QWidget;
    auto* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setSpacing(8);
    leftLayout->setContentsMargins(8, 8, 8, 8);

    // Title label
    auto* editorLabel = new QLabel("📝  C++ Code Editor");
    editorLabel->setStyleSheet("color:#89b4fa; font-weight:bold; font-size:14px;");
    leftLayout->addWidget(editorLabel);

    // Example loader combo
    m_exampleCombo = new QComboBox;
    for (const auto& [name, _] : k_examples) m_exampleCombo->addItem(name);
    connect(m_exampleCombo, QOverload<int>::of(&QComboBox::activated),
            this, &MainWindow::onLoadExample);
    leftLayout->addWidget(m_exampleCombo);

    // Code editor
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

    // Analyze button
    m_analyzeBtn = new QPushButton("🔍  Analyze Complexity");
    m_analyzeBtn->setObjectName("analyzeBtn");
    m_analyzeBtn->setCursor(Qt::PointingHandCursor);
    m_analyzeBtn->setMinimumHeight(42);
    connect(m_analyzeBtn, &QPushButton::clicked, this, &MainWindow::onAnalyze);
    leftLayout->addWidget(m_analyzeBtn);

    leftWidget->setMinimumWidth(420);

    // ── Right panel ─────────────────────────────────────────────────────
    auto* rightSplitter = new QSplitter(Qt::Vertical);

    // AST Tree
    m_astTree = new QTreeWidget;
    m_astTree->setHeaderLabels({"Node", "Depth", "Line"});
    m_astTree->setColumnWidth(0, 260);
    m_astTree->setColumnWidth(1, 60);
    m_astTree->setAlternatingRowColors(true);
    m_astTree->setFrameStyle(QFrame::NoFrame);

    rightSplitter->addWidget(m_astTree);

    // Bottom: chart + suggestions side-by-side
    auto* bottomWidget  = new QWidget;
    auto* bottomLayout  = new QHBoxLayout(bottomWidget);
    bottomLayout->setSpacing(8);
    bottomLayout->setContentsMargins(4, 8, 4, 4);

    // Chart — construct with initial chart to avoid null-scene crash
    auto* chartGroup  = new QGroupBox("📈  Complexity Growth Chart");
    auto* chartLayout = new QVBoxLayout(chartGroup);
    auto* initChart = ChartHelper::buildChart(ComplexityClass::O1);
    m_chartView = new QChartView(initChart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setMinimumHeight(260);
    m_chartView->setBackgroundBrush(QColor("#1e1e2e"));
    m_chartView->setFrameStyle(QFrame::NoFrame);
    chartLayout->addWidget(m_chartView);
    bottomLayout->addWidget(chartGroup, 3);

    // Suggestions
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

    // ── Main splitter ────────────────────────────────────────────────────
    auto* mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->addWidget(leftWidget);
    mainSplitter->addWidget(rightSplitter);
    mainSplitter->setStretchFactor(0, 2);
    mainSplitter->setStretchFactor(1, 3);
    mainSplitter->setHandleWidth(4);

    setCentralWidget(mainSplitter);

    // ── Status bar ───────────────────────────────────────────────────────
    m_complexityBadge = new QLabel("  Complexity: —  ");
    m_complexityBadge->setStyleSheet(
        "background:#313244; color:#cba6f7; border-radius:4px;"
        "padding:2px 8px; font-weight:bold; font-size:14px;");
    m_statsLabel = new QLabel("  Ready  ");
    m_statsLabel->setStyleSheet("color:#a6adc8;");
    statusBar()->addPermanentWidget(m_complexityBadge);
    statusBar()->addWidget(m_statsLabel);

    m_networkMgr = new QNetworkAccessManager(this);
    connect(m_networkMgr, &QNetworkAccessManager::finished, this, &MainWindow::onGeminiResponse);
    
}
// Note: initial chart is now set directly in QChartView constructor above

// ── Slots ────────────────────────────────────────────────────────────────────
void MainWindow::onLoadExample(int index) {
    if (index <= 0) return;
    m_editor->setPlainText(k_examples[(size_t)index].second);
    m_exampleCombo->setCurrentIndex(0);
}

void MainWindow::onAnalyze() {
    QString code = m_editor->toPlainText().trimmed();
    if (code.isEmpty()) {
        m_complexityBadge->setText("  O(1) — empty input  ");
        m_astTree->clear();
        m_suggList->clear();
        return;
    }

    Analyzer analyzer;
    AnalysisResult res = analyzer.analyze(code.toStdString());

    // Keep generating the AST tree for structural visualization
    m_astTree->clear();
    if (res.ast) {
        populateAST(res.ast, nullptr);
        m_astTree->expandAll();
    }

    m_complexityBadge->setText("  Complexity: Loading...  ");
    m_statsLabel->setText("  Analyzing with Gemini...  ");
    m_suggList->clear();
    m_analyzeBtn->setEnabled(false);

    QString apiKey = "AIzaSyB-gfsELZDn_0Pv37iDdk6uMPpRMV50wDU";
    QUrl url("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash:generateContent?key=" + apiKey);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject systemInstruction;
    QJsonObject sysParts;
    sysParts["text"] = "You are an expert C++ code analyzer. Analyze the provided code and return a JSON object with EXACTLY the following format:\n"
                       "{\n"
                       "  \"time_complexity\": \"O(...)\",\n"
                       "  \"space_complexity\": \"O(...)\",\n"
                       "  \"algorithms_used\": [\"Algo1\", \"Algo2\"],\n"
                       "  \"suggestions\": [\n"
                       "    { \"icon\": \"💡\", \"text\": \"Short suggestion...\", \"severity\": 1 } \n"
                       "  ]\n"
                       "}\n"
                       "Severity is 1 (minor) to 5 (critical).\n"
                       "Response must strictly be the raw JSON object.";
    QJsonArray sysPartsArray;
    sysPartsArray.append(sysParts);
    systemInstruction["parts"] = sysPartsArray;

    QJsonObject userMessage;
    QJsonObject userParts;
    userParts["text"] = code;
    QJsonArray userPartsArray;
    userPartsArray.append(userParts);
    userMessage["role"] = "user";
    userMessage["parts"] = userPartsArray;

    QJsonArray contents;
    contents.append(userMessage);

    QJsonObject generationConfig;
    generationConfig["responseMimeType"] = "application/json";

    QJsonObject payload;
    payload["contents"] = contents;
    payload["system_instruction"] = systemInstruction;
    payload["generationConfig"] = generationConfig;

    QJsonDocument doc(payload);
    m_networkMgr->post(request, doc.toJson());
}

void MainWindow::onGeminiResponse(QNetworkReply* reply) {
    m_analyzeBtn->setEnabled(true);
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        m_statsLabel->setText("  API Error: " + reply->errorString() + "  ");
        m_complexityBadge->setText("  Error  ");
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();

    if (root.contains("error")) {
        QString errMsg = root["error"].toObject()["message"].toString();
        m_statsLabel->setText("  API Error: " + errMsg + "  ");
        m_complexityBadge->setText("  Error  ");
        return;
    }

    QJsonArray candidates = root["candidates"].toArray();
    if (candidates.isEmpty()) {
        m_statsLabel->setText("  Error: Empty response  ");
        return;
    }

    QString responseText = candidates[0].toObject()["content"].toObject()["parts"].toArray()[0].toObject()["text"].toString();
    responseText = responseText.trimmed();
    
    if (responseText.startsWith("```json", Qt::CaseInsensitive)) {
        responseText.remove(0, 7);
        if (responseText.endsWith("```")) responseText.chop(3);
        responseText = responseText.trimmed();
    } else if (responseText.startsWith("```")) {
        responseText.remove(0, 3);
        if (responseText.endsWith("```")) responseText.chop(3);
        responseText = responseText.trimmed();
    }
    
    QJsonParseError parseErr;
    QJsonDocument respDoc = QJsonDocument::fromJson(responseText.toUtf8(), &parseErr);
    if (parseErr.error != QJsonParseError::NoError) {
        m_statsLabel->setText("  Error parsing JSON response  ");
        m_complexityBadge->setText("  Error  ");
        return;
    }

    QJsonObject result = respDoc.object();
    
    // Time Complexity
    QString tc = result["time_complexity"].toString();
    m_complexityBadge->setText("  Complexity: " + tc + "  ");

    // Space Complexity & Algorithms
    QString sc = result["space_complexity"].toString();
    QJsonArray algos = result["algorithms_used"].toArray();
    QStringList algoList;
    for (const auto& a : algos) algoList << a.toString();
    
    QString stats = "  🧠 " + algoList.join(" + ");
    if (!sc.isEmpty()) stats += "  │  Space: " + sc;
    stats += "  ";
    m_statsLabel->setText(stats);

    // Suggestions
    m_suggList->clear();
    QJsonArray suggs = result["suggestions"].toArray();
    if (suggs.isEmpty()) {
        auto* item = new QListWidgetItem("✅  No issues found — code looks efficient!");
        item->setForeground(QColor("#a6e3a1"));
        m_suggList->addItem(item);
    } else {
        for (const auto& s : suggs) {
            QJsonObject obj = s.toObject();
            QString icon = obj["icon"].toString();
            QString text = obj["text"].toString();
            int severity = obj["severity"].toInt();

            auto* item = new QListWidgetItem(icon + "  " + text);
            item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
            switch (severity) {
                case 5: item->setForeground(QColor("#f38ba8")); break;
                case 4: item->setForeground(QColor("#fab387")); break;
                case 3: item->setForeground(QColor("#f9e2af")); break;
                case 2: item->setForeground(QColor("#89dceb")); break;
                default:item->setForeground(QColor("#a6e3a1")); break;
            }
            m_suggList->addItem(item);
        }
    }

    // Chart update
    ComplexityClass cls = ComplexityClass::O1;
    if (tc.contains("2^N", Qt::CaseInsensitive) || tc.contains("exp", Qt::CaseInsensitive) || tc.contains("!", Qt::CaseInsensitive)) cls = ComplexityClass::ONK;
    else if (tc.contains("N^3", Qt::CaseInsensitive) || tc.contains("N³", Qt::CaseInsensitive)) cls = ComplexityClass::ON3;
    else if (tc.contains("N^2 log", Qt::CaseInsensitive) || tc.contains("N² log", Qt::CaseInsensitive)) cls = ComplexityClass::ON2LogN;
    else if (tc.contains("N^2", Qt::CaseInsensitive) || tc.contains("N²", Qt::CaseInsensitive) || tc.contains("V^2", Qt::CaseInsensitive) || tc.contains("E^2", Qt::CaseInsensitive)) cls = ComplexityClass::ON2;
    else if (tc.contains("N log", Qt::CaseInsensitive) || tc.contains("V log", Qt::CaseInsensitive) || tc.contains("E log", Qt::CaseInsensitive)) cls = ComplexityClass::ONLogN;
    else if (tc.contains("log", Qt::CaseInsensitive)) cls = ComplexityClass::OLogN;
    else if (tc.contains("N", Qt::CaseInsensitive) || tc.contains("V", Qt::CaseInsensitive) || tc.contains("E", Qt::CaseInsensitive)) cls = ComplexityClass::ON;
    updateChart(cls);
}

// ── Helpers ──────────────────────────────────────────────────────────────────
void MainWindow::populateAST(const shared_ptr<ASTNode>& node,
                              QTreeWidgetItem* parentItem) {
    if (!node) return;
    QString icon = nodeIcon(node->label);
    QString nodeLabel = icon + " " + QString::fromStdString(node->label);

    QTreeWidgetItem* item;
    if (!parentItem) {
        item = new QTreeWidgetItem(m_astTree, {nodeLabel, "", ""});
        item->setForeground(0, QColor("#89b4fa"));
    } else {
        item = new QTreeWidgetItem(parentItem,
            {nodeLabel,
             QString::number(node->depth),
             node->lineNumber > 0 ? QString::number(node->lineNumber) : "-"});

        // Color by node type
        QString lbl = QString::fromStdString(node->label);
        if (lbl == "for" || lbl == "while" || lbl == "do")
            item->setForeground(0, QColor("#fab387")); // orange = loops
        else if (lbl == "sort()")
            item->setForeground(0, QColor("#cba6f7")); // purple = sort
        else if (lbl.contains("binary") || lbl.contains("lower") || lbl.contains("upper"))
            item->setForeground(0, QColor("#89dceb")); // teal = binary search
        else if (lbl == "if" || lbl == "else")
            item->setForeground(0, QColor("#a6e3a1")); // green = conditionals
        else
            item->setForeground(0, QColor("#cdd6f4")); // default
    }

    for (const auto& child : node->children) {
        populateAST(child, item);
    }
}

void MainWindow::updateChart(ComplexityClass cls) {
    // Build new chart first, then swap — avoids null scene issues
    auto* newChart = ChartHelper::buildChart(cls);
    auto* oldChart = m_chartView->chart();
    m_chartView->setChart(newChart);  // Qt takes ownership
    m_chartView->setBackgroundBrush(QColor("#1e1e2e"));
    m_chartView->setFrameStyle(QFrame::NoFrame);
    // Safe to delete old chart only after it's no longer attached
    if (oldChart) delete oldChart;
}



QString MainWindow::nodeIcon(const string& label) const {
    if (label == "for")    return "🔁";
    if (label == "while")  return "🔄";
    if (label == "do")     return "🔂";
    if (label == "sort()") return "⬇";
    if (label == "if")     return "⑂";
    if (label == "else")   return "↩";
    if (label == "Program")return "📄";
    if (label.find("binary") != string::npos ||
        label.find("lower")  != string::npos ||
        label.find("upper")  != string::npos) return "🔍";
    return "⚙";
}
