#include "MainWindow.h"
#include "SyntaxHighlighter.h"
#include "ChartHelper.h"
#include "../src/PatternTracker.h"
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

// ── Example snippets ────────────────────────────────────────────────────────
static const std::vector<std::pair<QString, QString>> k_examples = {
    {"-- Load Example --", ""},
    // Sorting
    {"O(1) — Constant access",
        "int getFirst(int arr[], int n) {\n    return arr[0];\n}"},
    {"O(n) — Linear search",
        "int linearSearch(int arr[], int n, int x) {\n"
        "    for (int i = 0; i < n; i++) {\n"
        "        if (arr[i] == x) return i;\n"
        "    }\n    return -1;\n}"},
    {"O(log n) — Binary search",
        "#include <algorithm>\nbool found(std::vector<int>& v, int x) {\n"
        "    sort(v.begin(), v.end());\n"
        "    return binary_search(v.begin(), v.end(), x);\n}"},
    {"O(n log n) — std::sort",
        "#include <algorithm>\nvoid sortArray(std::vector<int>& v) {\n"
        "    sort(v.begin(), v.end());\n}"},
    {"O(n²) — Bubble Sort",
        "void bubbleSort(int arr[], int n) {\n"
        "    for (int i = 0; i < n; i++) {\n"
        "        for (int j = 0; j < n - i - 1; j++) {\n"
        "            if (arr[j] > arr[j+1])\n"
        "                std::swap(arr[j], arr[j+1]);\n"
        "        }\n    }\n}"},
    {"O(n²) — Selection Sort",
        "void selectionSort(int arr[], int n) {\n"
        "    for (int i = 0; i < n; i++) {\n"
        "        int minIdx = i;\n"
        "        for (int j = i+1; j < n; j++)\n"
        "            if (arr[j] < arr[minIdx]) minIdx = j;\n"
        "        std::swap(arr[i], arr[minIdx]);\n"
        "    }\n}"},
    {"O(n² log n) — Nested + sort",
        "void process(std::vector<int>& v, int n) {\n"
        "    for (int i = 0; i < n; i++) {\n"
        "        for (int j = 0; j < n; j++) {\n"
        "            sort(v.begin(), v.end());\n"
        "        }\n    }\n}"},
    {"O(n³) — Triple nested",
        "void tripleLoop(int n) {\n"
        "    for (int i = 0; i < n; i++) {\n"
        "        for (int j = 0; j < n; j++) {\n"
        "            for (int k = 0; k < n; k++) {\n"
        "                // O(n³)\n"
        "            }\n        }\n    }\n}"},
    // Graph
    {"BFS — O(V+E)",
        "#include <queue>\nvoid bfs(int start, std::vector<std::vector<int>>& adj, int n) {\n"
        "    std::vector<bool> visited(n, false);\n"
        "    std::queue<int> q;\n"
        "    q.push(start); visited[start] = true;\n"
        "    while (!q.empty()) {\n"
        "        int node = q.front(); q.pop();\n"
        "        for (int nb : adj[node])\n"
        "            if (!visited[nb]) { visited[nb]=true; q.push(nb); }\n"
        "    }\n}"},
    {"DFS Recursive — O(V+E)",
        "void dfs(int node, std::vector<std::vector<int>>& adj,\n"
        "         std::vector<bool>& visited) {\n"
        "    visited[node] = true;\n"
        "    for (int nb : adj[node])\n"
        "        if (!visited[nb]) dfs(nb, adj, visited);\n"
        "}"},
    // DP
    {"DP — Fibonacci (Tabulation)",
        "int fib(int n) {\n"
        "    std::vector<int> dp(n+1, 0);\n"
        "    dp[0] = 0; dp[1] = 1;\n"
        "    for (int i = 2; i <= n; i++)\n"
        "        dp[i] = dp[i-1] + dp[i-2];\n"
        "    return dp[n];\n}"},
    {"DP — 0/1 Knapsack O(nW)",
        "int knapsack(int W, int wt[], int val[], int n) {\n"
        "    std::vector<std::vector<int>> dp(n+1, std::vector<int>(W+1, 0));\n"
        "    for (int i = 1; i <= n; i++) {\n"
        "        for (int w = 0; w <= W; w++) {\n"
        "            dp[i][w] = dp[i-1][w];\n"
        "            if (wt[i-1] <= w)\n"
        "                dp[i][w] = std::max(dp[i][w], dp[i-1][w-wt[i-1]] + val[i-1]);\n"
        "        }\n    }\n    return dp[n][W];\n}"},
    // Hashing / Two-pointer
    {"Hashing — Two Sum O(n)",
        "#include <unordered_map>\nstd::pair<int,int> twoSum(std::vector<int>& nums, int target) {\n"
        "    std::unordered_map<int,int> mp;\n"
        "    for (int i = 0; i < (int)nums.size(); i++) {\n"
        "        if (mp.count(target - nums[i]))\n"
        "            return {mp[target-nums[i]], i};\n"
        "        mp[nums[i]] = i;\n"
        "    }\n    return {-1,-1};\n}"},
    {"Greedy — Min Heap (Dijkstra style)",
        "#include <queue>\nvoid dijkstra(int src, std::vector<std::pair<int,int>> adj[], int n) {\n"
        "    std::vector<int> dist(n, INT_MAX);\n"
        "    std::priority_queue<std::pair<int,int>,\n"
        "        std::vector<std::pair<int,int>>, std::greater<>> pq;\n"
        "    dist[src] = 0; pq.push({0, src});\n"
        "    while (!pq.empty()) {\n"
        "        auto [d, u] = pq.top(); pq.pop();\n"
        "        for (auto [w, v] : adj[u])\n"
        "            if (dist[u]+w < dist[v]) { dist[v]=dist[u]+w; pq.push({dist[v],v}); }\n"
        "    }\n}"},
    {"Bit Manipulation — Count set bits",
        "int countBits(int n) {\n"
        "    int count = 0;\n"
        "    while (n) { count += n & 1; n >>= 1; }\n"
        "    return count;\n    // or: return __builtin_popcount(n);\n}"},
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
QSplitter::handle { background: #313244; }
QGroupBox {
    border: 1px solid #313244;
    border-radius: 6px;
    margin-top: 16px;
    padding-top: 8px;
    color: #89b4fa;
    font-weight: bold;
}
QGroupBox::title {
    subcontrol-origin: margin;
    left: 10px;
    top: -8px;
    background: #1e1e2e;
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
    auto* astGroup = new QGroupBox("🌳  Abstract Syntax Tree");
    auto* astLayout = new QVBoxLayout(astGroup);
    m_astTree = new QTreeWidget;
    m_astTree->setHeaderLabels({"Node", "Depth", "Line"});
    m_astTree->setColumnWidth(0, 260);
    m_astTree->setColumnWidth(1, 60);
    m_astTree->setAlternatingRowColors(true);
    astLayout->addWidget(m_astTree);
    rightSplitter->addWidget(astGroup);

    // Bottom: chart + suggestions side-by-side
    auto* bottomWidget  = new QWidget;
    auto* bottomLayout  = new QHBoxLayout(bottomWidget);
    bottomLayout->setSpacing(8);
    bottomLayout->setContentsMargins(0, 0, 0, 0);

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

    // ── Complexity badge ────────────────────────────────────────────────
    QString label = QString::fromStdString(res.complexity);
    m_complexityBadge->setText("  Complexity: " + label + "  ");

    // ── Stats label with algorithm name ─────────────────────────────────
    QString algoName = QString::fromStdString(algorithmName(res.primaryAlgorithm));
    QString stats;
    if (res.primaryAlgorithm != AlgorithmKind::NONE &&
        res.primaryAlgorithm != AlgorithmKind::UNKNOWN) {
        stats = "  🧠 " + algoName;
        if (res.secondaryAlgorithm != AlgorithmKind::NONE)
            stats += " + " + QString::fromStdString(algorithmName(res.secondaryAlgorithm));
        stats += "  │";
    }
    stats += QString("  for×%1  while×%2  depth=%3")
        .arg(res.forLoops)
        .arg(res.whileLoops)
        .arg(res.maxDepth);
    if (res.hasSort)       stats += "  sort✓";
    if (res.hasBinSearch)  stats += "  bsearch✓";
    if (res.hasHashMap)    stats += "  hash✓";
    if (res.hasQueue)      stats += "  queue✓";
    if (res.hasMemo)       stats += "  dp✓";
    if (res.hasBitOp)      stats += "  bit✓";
    m_statsLabel->setText(stats);


    // ── AST tree ────────────────────────────────────────────────────────
    m_astTree->clear();
    if (res.ast) {
        populateAST(res.ast, nullptr);
        m_astTree->expandAll();
    }

    // ── Chart ────────────────────────────────────────────────────────────
    updateChart(res.complexityClass);

    // ── Suggestions ──────────────────────────────────────────────────────
    updateSuggestions(res.suggestions);

    // Button pulse animation
    auto* anim = new QPropertyAnimation(m_analyzeBtn, "geometry");
    anim->setDuration(120);
    QRect r = m_analyzeBtn->geometry();
    anim->setKeyValueAt(0.0, r);
    anim->setKeyValueAt(0.5, r.adjusted(2, 2, -2, -2));
    anim->setKeyValueAt(1.0, r);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

// ── Helpers ──────────────────────────────────────────────────────────────────
void MainWindow::populateAST(const std::shared_ptr<ASTNode>& node,
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

void MainWindow::updateSuggestions(const std::vector<Suggestion>& suggestions) {
    m_suggList->clear();
    if (suggestions.empty()) {
        auto* item = new QListWidgetItem("✅  No issues found — code looks efficient!");
        item->setForeground(QColor("#a6e3a1"));
        m_suggList->addItem(item);
        return;
    }
    for (const auto& s : suggestions) {
        QString text = QString::fromStdString(s.icon + "  " + s.text);
        auto* item = new QListWidgetItem(text);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        // Color by severity
        switch (s.severity) {
        case 5: item->setForeground(QColor("#f38ba8")); break; // red
        case 4: item->setForeground(QColor("#fab387")); break; // orange
        case 3: item->setForeground(QColor("#f9e2af")); break; // yellow
        case 2: item->setForeground(QColor("#89dceb")); break; // cyan
        default:item->setForeground(QColor("#a6e3a1")); break; // green
        }
        m_suggList->addItem(item);
    }
}

QString MainWindow::nodeIcon(const std::string& label) const {
    if (label == "for")    return "🔁";
    if (label == "while")  return "🔄";
    if (label == "do")     return "🔂";
    if (label == "sort()") return "⬇";
    if (label == "if")     return "⑂";
    if (label == "else")   return "↩";
    if (label == "Program")return "📄";
    if (label.find("binary") != std::string::npos ||
        label.find("lower")  != std::string::npos ||
        label.find("upper")  != std::string::npos) return "🔍";
    return "⚙";
}
