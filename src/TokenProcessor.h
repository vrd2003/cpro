#pragma once
#include <string>
#include <vector>

enum class TokenType {
    // ── Control flow ───────────────────────────────────────────────────────
    FOR, WHILE, DO, IF, ELSE, RETURN,

    // ── Sorting ────────────────────────────────────────────────────────────
    SORT,           // std::sort, stable_sort
    QSORT,          // qsort (C-style)

    // ── Searching ──────────────────────────────────────────────────────────
    BINARY_SEARCH,  // binary_search / lower_bound / upper_bound / equal_range
    FIND,           // std::find (linear)

    // ── Graph / Tree traversal ─────────────────────────────────────────────
    QUEUE_OP,       // queue, BFS signal
    STACK_OP,       // stack (DFS stack-based)
    DEQUE_OP,       // deque

    // ── DP / Memoization ──────────────────────────────────────────────────
    MEMO_MAP,       // memo / dp array indexing pattern: dp[i], memo[key]
    DP_ARRAY,       // vector<vector<int>> dp or int dp[...][...]

    // ── Greedy/Heap ────────────────────────────────────────────────────────
    PRIORITY_QUEUE, // priority_queue
    PUSH_HEAP,      // push_heap, pop_heap, make_heap

    // ── Hashing ────────────────────────────────────────────────────────────
    HASH_MAP,       // unordered_map / unordered_set
    MAP_OP,         // map / set (ordered → log n)

    // ── Two Pointer / Sliding Window helpers ──────────────────────────────
    SWAP_OP,        // std::swap (hinting at in-place)
    REVERSE_OP,     // std::reverse

    // ── Recursion ──────────────────────────────────────────────────────────
    RECURSIVE_CALL, // function calling itself (detected by name match)

    // ── Bit manipulation ──────────────────────────────────────────────────
    BIT_OP,         // __builtin_popcount, __builtin_clz, >>1, &1 hints

    // ── Structure ──────────────────────────────────────────────────────────
    FUNC_DEF,       // function definition
    OPEN_BRACE, CLOSE_BRACE, OPEN_PAREN, CLOSE_PAREN,
    OTHER
};

struct Token {
    TokenType   type;
    std::string text;
    int         lineNumber;
};

class TokenProcessor {
public:
    std::vector<Token> tokenize(const std::string& code);

    // Expose function names found (for recursion detection)
    std::vector<std::string> functionNames; // populated during tokenize

private:
    TokenType classifyWord(const std::string& word,
                           const std::string& line,
                           const std::string& nextWord) const;
};
