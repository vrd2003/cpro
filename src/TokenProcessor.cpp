#include "TokenProcessor.h"
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

static string trim(const string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    size_t e = s.find_last_not_of(" \t\r\n");
    return (b == string::npos) ? "" : s.substr(b, e - b + 1);
}
static string stripComment(const string& line) {
    auto p = line.find("//");
    return (p != string::npos) ? line.substr(0, p) : line;
}
static bool contains(const string& h, const string& n) {
    return h.find(n) != string::npos;
}

TokenType TokenProcessor::classifyWord(const string& w,
                                        const string& line,
                                        const string& next) const {
    // ── Control flow ──────────────────────────────────────────────────────
    if (w == "for") {
        return TokenType::FOR;
    }
    if (w == "while") {
        return TokenType::WHILE;
    }
    if (w == "do") {
        return TokenType::DO;
    }
    if (w == "if") {
        return TokenType::IF;
    }
    if (w == "else") {
        return TokenType::ELSE;
    }
    if (w == "return") {
        return TokenType::RETURN;
    }

    // ── Sorting ───────────────────────────────────────────────────────────
    if ((w == "sort" || w == "stable_sort") && contains(line, w + "(")) {
        return TokenType::SORT;
    }
    if (w == "qsort" && contains(line, "qsort(")) {
        return TokenType::QSORT;
    }

    // ── Searching ─────────────────────────────────────────────────────────
    if ((w == "binary_search" || w == "lower_bound" ||
         w == "upper_bound"   || w == "equal_range") && contains(line, w + "(")) {
        return TokenType::BINARY_SEARCH;
    }
    if ((w == "find" || w == "find_if") && contains(line, w + "(")) {
        return TokenType::FIND;
    }

    // ── Containers / Graph ────────────────────────────────────────────────
    if (w == "queue") {
        return TokenType::QUEUE_OP;
    }
    if (w == "deque") {
        return TokenType::DEQUE_OP;
    }
    if (w == "stack") {
        return TokenType::STACK_OP;
    }
    if (w == "priority_queue") {
        return TokenType::PRIORITY_QUEUE;
    }
    if (w == "push_heap" || w == "pop_heap" || w == "make_heap") {
        return TokenType::PUSH_HEAP;
    }

    // ── Hashing ───────────────────────────────────────────────────────────
    if (w == "unordered_map" || w == "unordered_set" ||
        w == "unordered_multimap" || w == "unordered_multiset") {
        return TokenType::HASH_MAP;
    }
    if (w == "map" || w == "set" || w == "multimap" || w == "multiset") {
        return TokenType::MAP_OP;
    }

    // ── Utilities ─────────────────────────────────────────────────────────
    if (w == "swap" && contains(line, "swap(")) {
        return TokenType::SWAP_OP;
    }
    if (w == "reverse" && contains(line, "reverse(")) {
        return TokenType::REVERSE_OP;
    }

    // ── DP / Memo hints ───────────────────────────────────────────────────
    // dp[, memo[, cache[ patterns
    if ((w == "dp" || w == "memo" || w == "cache" || w == "f") &&
        contains(line, w + "[")) {
        return TokenType::MEMO_MAP;
    }

    // ── Bit manipulation ──────────────────────────────────────────────────
    if (w == "__builtin_popcount" || w == "__builtin_clz" ||
        w == "__builtin_ctz"      || w == "__builtin_parity") {
        return TokenType::BIT_OP;
    }

    // ── Function definitions (word followed by '(' not preceded by control kw)
    if (!w.empty() && isupper((unsigned char)w[0]) == false &&
        isalpha((unsigned char)w[0]) && contains(line, w + "(")) {
        // skip known false positives already classified
        return TokenType::FUNC_DEF;
    }

    return TokenType::OTHER;
}

vector<Token> TokenProcessor::tokenize(const string& code) {
    functionNames.clear();
    vector<Token> tokens;
    istringstream stream(code);
    string line;
    int lineNum = 0;
    bool inBlockComment = false;

    // First pass: collect function names for recursion detection
    // A function definition looks like:  returnType funcname(
    {
        istringstream pre(code);
        string pline;
        while (getline(pre, pline)) {
            string cl = trim(stripComment(pline));
            // very simple heuristic: non-keyword word before '(' on a line
            // that has a '{' somewhere (definition, not just call)
            // We'll collect all word-before-paren candidates
        }
    }

    while (getline(stream, line)) {
        ++lineNum;

        if (inBlockComment) {
            if (line.find("*/") != string::npos) {
                inBlockComment = false;
            }
            continue;
        }
        auto bc = line.find("/*");
        if (bc != string::npos) {
            if (line.find("*/", bc + 2) == string::npos) {
                inBlockComment = true; 
                continue;
            }
        }

        string clean = trim(stripComment(line));
        if (clean.empty()) {
            continue;
        }

        // Emit brace/paren tokens (character-level)
        for (char c : clean) {
            Token t; 
            t.lineNumber = lineNum;
            if (c == '{') { 
                t.type = TokenType::OPEN_BRACE;  
                t.text = "{"; 
                tokens.push_back(t); 
            } else if (c == '}') { 
                t.type = TokenType::CLOSE_BRACE; 
                t.text = "}"; 
                tokens.push_back(t); 
            } else if (c == '(') { 
                t.type = TokenType::OPEN_PAREN;  
                t.text = "("; 
                tokens.push_back(t); 
            } else if (c == ')') { 
                t.type = TokenType::CLOSE_PAREN; 
                t.text = ")"; 
                tokens.push_back(t); 
            }
        }

        // Extract and classify words
        vector<string> words;
        string word;
        for (size_t i = 0; i <= clean.size(); ++i) {
            char c = (i < clean.size()) ? clean[i] : '\0';
            if (isalnum((unsigned char)c) || c == '_') {
                word += c;
            } else {
                if (!word.empty()) { words.push_back(word); word.clear(); }
            }
        }

        // Bit-op heuristic: >> 1 or & 1 or << patterns on the line
        bool hasBitPattern = (clean.find(">> 1") != string::npos ||
                              clean.find(">>1")  != string::npos ||
                              clean.find("<< 1")  != string::npos ||
                              clean.find("& 1")   != string::npos ||
                              clean.find("&1")    != string::npos ||
                              clean.find("^ ")    != string::npos);

        if (hasBitPattern) {
            Token t; t.type = TokenType::BIT_OP; t.text = "bit"; t.lineNumber = lineNum;
            tokens.push_back(t);
        }

        for (size_t i = 0; i < words.size(); ++i) {
            const string& w    = words[i];
            const string& nxt  = (i + 1 < words.size()) ? words[i+1] : "";
            TokenType tt = classifyWord(w, clean, nxt);

            if (tt == TokenType::OTHER || tt == TokenType::OPEN_BRACE ||
                tt == TokenType::CLOSE_BRACE || tt == TokenType::OPEN_PAREN ||
                tt == TokenType::CLOSE_PAREN) {
                continue;
            }

            Token t; 
            t.type = tt; 
            t.text = w; 
            t.lineNumber = lineNum;
            tokens.push_back(t);

            // Collect function names: word followed by '(' that is FUNC_DEF
            if (tt == TokenType::FUNC_DEF) {
                static const vector<string> kKeywords = {
                    "for", "while", "do", "if", "else", "return", "switch", "case",
                    "sort", "find", "swap", "reverse", "push", "pop"
                };
                bool isKw = false;
                for (const auto& k : kKeywords) {
                    if (w == k) { 
                        isKw = true; 
                        break; 
                    }
                }
                if (!isKw && w.size() > 1) {
                    functionNames.push_back(w);
                }
            }
        }
    }
    return tokens;
}
