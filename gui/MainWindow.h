#pragma once
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTreeWidget>
#include <QListWidget>
#include <QLabel>
#include <QComboBox>
#include <QSplitter>
#include <QtCharts/QChartView>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
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

private:
    // Left panel
    QPlainTextEdit*        m_editor;
    SyntaxHighlighter*     m_highlighter;
    QPushButton*           m_analyzeBtn;
    QComboBox*             m_exampleCombo;

    // Right panel
    QTreeWidget*             m_astTree;
    QChartView*              m_chartView;
    QListWidget*             m_suggList;

    // Status bar widgets
    QLabel*            m_complexityBadge;
    QLabel*            m_statsLabel;

    // Network & API
    QNetworkAccessManager* m_networkMgr;

    // Helpers
    void buildUi();
    void applyTheme();
    void populateAST(const std::shared_ptr<ASTNode>& node,
                     QTreeWidgetItem* parentItem);
    void updateChart(ComplexityClass cls);
    QString nodeIcon(const std::string& label) const;

private slots:
    void onGeminiResponse(QNetworkReply* reply);

};
