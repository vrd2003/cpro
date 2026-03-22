#pragma once
#include <string>
#include <vector>

using namespace std;

enum class TokenType {
    // ── Control flow ───────────────────────────────────────────────────────
    FOR, WHILE, DO, IF, ELSE, RETURN,

    // ── Sorting ────────────────────────────────────────────────────────────
    SORT,           // sort, stable_sort
    QSORT,          // qsort (C-style)

    // ── Searching ──────────────────────────────────────────────────────────
    BINARY_SEARCH,  // binary_search / lower_bound / upper_bound / equal_range
    FIND,           // find (linear)

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
    SWAP_OP,        // swap (hinting at in-place)
    REVERSE_OP,     // reverse

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
    TokenType type;
    string text;
    int lineNumber;
};

class TokenProcessor {
public:
    vector<Token> tokenize(const string& code);

    // Expose function names found (for recursion detection)
    vector<string> functionNames; // populated during tokenize

private:
    TokenType classifyWord(const string& word,const string& line,const string& nextWord) const;
};
