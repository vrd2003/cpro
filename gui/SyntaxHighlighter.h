#pragma once
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <vector>

using namespace std;

class SyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    explicit SyntaxHighlighter(QTextDocument* parent = nullptr);
protected:
    void highlightBlock(const QString& text) override;
private:
    struct Rule {
        QRegularExpression pattern;
        QTextCharFormat    format;
    };
    vector<Rule>  rules;
    QTextCharFormat    blockCommentFormat;
    QRegularExpression commentStart;
    QRegularExpression commentEnd;
};
