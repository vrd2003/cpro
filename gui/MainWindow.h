#pragma once
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTreeWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QTabWidget>
#include <QLabel>
#include <QComboBox>
#include <QSplitter>
#include <QJsonObject>
#include <QtCharts/QChartView>
#include "GeminiClient.h"
#include "../src/Analyzer.h"
#include "../src/ComplexityClass.h"

class SyntaxHighlighter;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onAnalyze();
    void onLoadExample(int index);
    void onChangeApiKey();

private:
    // ── Left panel ───────────────────────────────────────────────────────────
    QPlainTextEdit*    m_editor;
    SyntaxHighlighter* m_highlighter;
    QPushButton*       m_analyzeBtn;
    QPushButton*       m_apiKeyBtn;
    QComboBox*         m_exampleCombo;

    // ── Right panel ──────────────────────────────────────────────────────────
    QTabWidget*   m_topTabs;
    QTreeWidget*  m_astTree;
    QTableWidget* m_funcTable;
    QChartView*   m_chartView;
    QListWidget*  m_suggList;

    // ── Status bar ───────────────────────────────────────────────────────────
    QLabel* m_complexityBadge;
    QLabel* m_statsLabel;

    // ── Gemini networking (owned, SRP-isolated) ───────────────────────────────
    GeminiClient* m_gemini;

    // ── UI helpers ───────────────────────────────────────────────────────────
    void buildUi();
    void applyTheme();
    void populateAST(const std::shared_ptr<ASTNode>& node,
                     QTreeWidgetItem* parentItem);
    void updateChart(ComplexityClass cls);
    QString nodeIcon(const std::string& label) const;

    // ── Analysis result helpers ───────────────────────────────────────────────
    // Classifies a Gemini time-complexity string into a ComplexityClass enum.
    ComplexityClass classifyComplexity(const QString& tc) const;
    // Applies a parsed Gemini JSON result to all UI panels.
    void applyAnalysisResult(const QJsonObject& result);
};
