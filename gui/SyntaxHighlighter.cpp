#include "SyntaxHighlighter.h"
#include <QColor>

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
    , commentStart(QStringLiteral("/\\*"))
    , commentEnd(QStringLiteral("\\*/"))
{
    // ── Keywords ─────────────────────────────────────────────────────────
    QTextCharFormat kwFmt;
    kwFmt.setForeground(QColor("#569CD6"));  // blue
    kwFmt.setFontWeight(QFont::Bold);

    const QStringList keywords = {
        "\\bfor\\b","\\bwhile\\b","\\bdo\\b","\\bif\\b","\\belse\\b",
        "\\bswitch\\b","\\bcase\\b","\\bbreak\\b","\\bcontinue\\b","\\breturn\\b",
        "\\bint\\b","\\blong\\b","\\bfloat\\b","\\bdouble\\b","\\bbool\\b",
        "\\bchar\\b","\\bvoid\\b","\\bauto\\b","\\bconst\\b","\\bstatic\\b",
        "\\bnamespace\\b","\\busing\\b","\\bstruct\\b","\\bclass\\b",
        "\\btemplate\\b","\\btypename\\b","\\bnew\\b","\\bdelete\\b",
        "\\bpublic\\b","\\bprivate\\b","\\bprotected\\b"
    };
    for (const QString& kw : keywords) {
        rules.push_back({QRegularExpression(kw), kwFmt});
    }

    // ── STL / DSA types ──────────────────────────────────────────────────
    QTextCharFormat stlFmt;
    stlFmt.setForeground(QColor("#4EC9B0")); // teal
    const QStringList stlTypes = {
        "\\bvector\\b","\\bstack\\b","\\bqueue\\b","\\bdeque\\b",
        "\\bmap\\b","\\bset\\b","\\bunordered_map\\b","\\bunordered_set\\b",
        "\\bpriority_queue\\b","\\blist\\b","\\bstring\\b","\\bpair\\b",
        "\\barray\\b","\\bbitset\\b","\\biterator\\b"
    };
    for (const QString& t : stlTypes) {
        rules.push_back({QRegularExpression(t), stlFmt});
    }

    // ── DSA function calls ────────────────────────────────────────────────
    QTextCharFormat fnFmt;
    fnFmt.setForeground(QColor("#DCDCAA")); // yellow
    const QStringList fns = {
        "\\bsort\\b","\\bstable_sort\\b","\\bbinary_search\\b",
        "\\blower_bound\\b","\\bupper_bound\\b","\\breverse\\b",
        "\\bmax\\b","\\bmin\\b","\\bfind\\b","\\bcount\\b",
        "\\baccumulate\\b","\\bpush_back\\b","\\bpop_back\\b",
        "\\bpush\\b","\\bpop\\b","\\btop\\b","\\bfront\\b","\\bback\\b",
        "\\bemplace\\b","\\binsert\\b","\\berase\\b","\\bpush_heap\\b",
        "\\bpop_heap\\b","\\bmake_heap\\b"
    };
    for (const QString& fn : fns) {
        rules.push_back({QRegularExpression(fn), fnFmt});
    }

    // ── Numbers ───────────────────────────────────────────────────────────
    QTextCharFormat numFmt;
    numFmt.setForeground(QColor("#B5CEA8")); // light green
    rules.push_back({QRegularExpression(QStringLiteral("\\b[0-9]+(\\.[0-9]+)?\\b")), numFmt});

    // ── String literals ───────────────────────────────────────────────────
    QTextCharFormat strFmt;
    strFmt.setForeground(QColor("#CE9178")); // orange/brown
    rules.push_back({QRegularExpression(QStringLiteral("\".*\"")), strFmt});

    // ── Preprocessor ─────────────────────────────────────────────────────
    QTextCharFormat prepFmt;
    prepFmt.setForeground(QColor("#9B9B9B")); // grey
    rules.push_back({QRegularExpression(QStringLiteral("^\\s*#[^\n]*")), prepFmt});

    // ── Single-line comment ───────────────────────────────────────────────
    QTextCharFormat commentFmt;
    commentFmt.setForeground(QColor("#6A9955")); // green
    commentFmt.setFontItalic(true);
    rules.push_back({QRegularExpression(QStringLiteral("//[^\n]*")), commentFmt});

    // Block comment format (used in highlightBlock for multi-line)
    blockCommentFormat.setForeground(QColor("#6A9955"));
    blockCommentFormat.setFontItalic(true);
}

void SyntaxHighlighter::highlightBlock(const QString& text) {
    // Apply single-line rules
    for (const auto& rule : rules) {
        QRegularExpressionMatchIterator it = rule.pattern.globalMatch(text);
        while (it.hasNext()) {
            auto m = it.next();
            setFormat((int)m.capturedStart(), (int)m.capturedLength(), rule.format);
        }
    }

    // Multi-line block comments
    setCurrentBlockState(0);
    int start = 0;
    if (previousBlockState() != 1) {
        start = (int)commentStart.match(text).capturedStart();
    }
    while (start >= 0) {
        auto mEnd = commentEnd.match(text, start);
        int end, len;
        if (mEnd.hasMatch()) {
            end = (int)mEnd.capturedStart();
            len = end - start + (int)mEnd.capturedLength();
        } else {
            setCurrentBlockState(1);
            len = text.length() - start;
        }
        setFormat(start, len, blockCommentFormat);
        start = (int)commentStart.match(text, start + len).capturedStart();
    }
}
