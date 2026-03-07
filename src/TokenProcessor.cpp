#include "TokenProcessor.h"
#include <sstream>
#include <algorithm>
#include <cctype>

static std::string trim(const std::string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    size_t e = s.find_last_not_of(" \t\r\n");
    return (b == std::string::npos) ? "" : s.substr(b, e - b + 1);
}
static std::string stripComment(const std::string& line) {
    auto p = line.find("//");
    return (p != std::string::npos) ? line.substr(0, p) : line;
}
static bool contains(const std::string& h, const std::string& n) {
    return h.find(n) != std::string::npos;
}

TokenType TokenProcessor::classifyWord(const std::string& w,
                                        const std::string& line,
                                        const std::string& next) const {
    // ── Control flow ──────────────────────────────────────────────────────
    if (w == "for")    return TokenType::FOR;
    if (w == "while")  return TokenType::WHILE;
    if (w == "do")     return TokenType::DO;
    if (w == "if")     return TokenType::IF;
    if (w == "else")   return TokenType::ELSE;
    if (w == "return") return TokenType::RETURN;

    // ── Sorting ───────────────────────────────────────────────────────────
    if ((w == "sort" || w == "stable_sort") && contains(line, w + "("))
        return TokenType::SORT;
    if (w == "qsort" && contains(line, "qsort("))
        return TokenType::QSORT;

    // ── Searching ─────────────────────────────────────────────────────────
    if ((w == "binary_search" || w == "lower_bound" ||
         w == "upper_bound"   || w == "equal_range") && contains(line, w + "("))
        return TokenType::BINARY_SEARCH;
    if ((w == "find" || w == "find_if") && contains(line, w + "("))
        return TokenType::FIND;

    // ── Containers / Graph ────────────────────────────────────────────────
    if (w == "queue")    return TokenType::QUEUE_OP;
    if (w == "deque")    return TokenType::DEQUE_OP;
    if (w == "stack")    return TokenType::STACK_OP;
    if (w == "priority_queue") return TokenType::PRIORITY_QUEUE;
    if (w == "push_heap" || w == "pop_heap" || w == "make_heap")
        return TokenType::PUSH_HEAP;

    // ── Hashing ───────────────────────────────────────────────────────────
    if (w == "unordered_map" || w == "unordered_set" ||
        w == "unordered_multimap" || w == "unordered_multiset")
        return TokenType::HASH_MAP;
    if (w == "map" || w == "set" || w == "multimap" || w == "multiset")
        return TokenType::MAP_OP;

    // ── Utilities ─────────────────────────────────────────────────────────
    if (w == "swap" && contains(line, "swap("))   return TokenType::SWAP_OP;
    if (w == "reverse" && contains(line, "reverse(")) return TokenType::REVERSE_OP;

    // ── DP / Memo hints ───────────────────────────────────────────────────
    // dp[, memo[, cache[ patterns
    if ((w == "dp" || w == "memo" || w == "cache" || w == "f") &&
        contains(line, w + "["))
        return TokenType::MEMO_MAP;

    // ── Bit manipulation ──────────────────────────────────────────────────
    if (w == "__builtin_popcount" || w == "__builtin_clz" ||
        w == "__builtin_ctz"      || w == "__builtin_parity")
        return TokenType::BIT_OP;

    // ── Function definitions (word followed by '(' not preceded by control kw)
    if (!w.empty() && std::isupper((unsigned char)w[0]) == false &&
        std::isalpha((unsigned char)w[0]) && contains(line, w + "(")) {
        // skip known false positives already classified
        return TokenType::FUNC_DEF;
    }

    return TokenType::OTHER;
}

std::vector<Token> TokenProcessor::tokenize(const std::string& code) {
    functionNames.clear();
    std::vector<Token> tokens;
    std::istringstream stream(code);
    std::string line;
    int lineNum = 0;
    bool inBlockComment = false;

    // First pass: collect function names for recursion detection
    // A function definition looks like:  returnType funcname(
    {
        std::istringstream pre(code);
        std::string pline;
        while (std::getline(pre, pline)) {
            std::string cl = trim(stripComment(pline));
            // very simple heuristic: non-keyword word before '(' on a line
            // that has a '{' somewhere (definition, not just call)
            // We'll collect all word-before-paren candidates
        }
    }

    while (std::getline(stream, line)) {
        ++lineNum;

        if (inBlockComment) {
            if (line.find("*/") != std::string::npos) inBlockComment = false;
            continue;
        }
        auto bc = line.find("/*");
        if (bc != std::string::npos) {
            if (line.find("*/", bc + 2) == std::string::npos) {
                inBlockComment = true; continue;
            }
        }

        std::string clean = trim(stripComment(line));
        if (clean.empty()) continue;

        // Emit brace/paren tokens (character-level)
        for (char c : clean) {
            Token t; t.lineNumber = lineNum;
            if      (c == '{') { t.type = TokenType::OPEN_BRACE;  t.text = "{"; tokens.push_back(t); }
            else if (c == '}') { t.type = TokenType::CLOSE_BRACE; t.text = "}"; tokens.push_back(t); }
            else if (c == '(') { t.type = TokenType::OPEN_PAREN;  t.text = "("; tokens.push_back(t); }
            else if (c == ')') { t.type = TokenType::CLOSE_PAREN; t.text = ")"; tokens.push_back(t); }
        }

        // Extract and classify words
        std::vector<std::string> words;
        std::string word;
        for (size_t i = 0; i <= clean.size(); ++i) {
            char c = (i < clean.size()) ? clean[i] : '\0';
            if (std::isalnum((unsigned char)c) || c == '_') {
                word += c;
            } else {
                if (!word.empty()) { words.push_back(word); word.clear(); }
            }
        }

        // Bit-op heuristic: >> 1 or & 1 or << patterns on the line
        bool hasBitPattern = (clean.find(">> 1") != std::string::npos ||
                              clean.find(">>1")  != std::string::npos ||
                              clean.find("<< 1")  != std::string::npos ||
                              clean.find("& 1")   != std::string::npos ||
                              clean.find("&1")    != std::string::npos ||
                              clean.find("^ ")    != std::string::npos);

        if (hasBitPattern) {
            Token t; t.type = TokenType::BIT_OP; t.text = "bit"; t.lineNumber = lineNum;
            tokens.push_back(t);
        }

        for (size_t i = 0; i < words.size(); ++i) {
            const std::string& w    = words[i];
            const std::string& nxt  = (i + 1 < words.size()) ? words[i+1] : "";
            TokenType tt = classifyWord(w, clean, nxt);

            if (tt == TokenType::OTHER || tt == TokenType::OPEN_BRACE ||
                tt == TokenType::CLOSE_BRACE || tt == TokenType::OPEN_PAREN ||
                tt == TokenType::CLOSE_PAREN)
                continue;

            Token t; t.type = tt; t.text = w; t.lineNumber = lineNum;
            tokens.push_back(t);

            // Collect function names: word followed by '(' that is FUNC_DEF
            if (tt == TokenType::FUNC_DEF) {
                static const std::vector<std::string> kKeywords = {
                    "for","while","do","if","else","return","switch","case",
                    "sort","find","swap","reverse","push","pop"};
                bool isKw = false;
                for (auto& k : kKeywords) if (w == k) { isKw = true; break; }
                if (!isKw && w.size() > 1)
                    functionNames.push_back(w);
            }
        }
    }
    return tokens;
}
