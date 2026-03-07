#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

// Enumeration of all named DSA algorithms / paradigms the tool can detect
enum class AlgorithmKind {
    NONE,
    // Sorting
    BUBBLE_SORT,
    SELECTION_SORT,
    INSERTION_SORT,
    MERGE_SORT,
    QUICK_SORT,
    HEAP_SORT,
    STL_SORT,
    COUNTING_SORT,      // hinted: loop building freq table
    // Searching
    LINEAR_SEARCH,
    BINARY_SEARCH,
    // Graph / Tree
    BFS,                // queue + visited loop
    DFS_RECURSIVE,      // recursive function + adjacency list
    DFS_ITERATIVE,      // stack-based
    // Dynamic Programming
    MEMOIZATION,
    TABULATION,
    // Greedy
    GREEDY_HEAP,        // priority_queue usage
    // Divide & Conquer
    DIVIDE_AND_CONQUER, // recursive halving hints
    // Two pointer / Sliding window
    TWO_POINTER,
    SLIDING_WINDOW,
    // Hashing
    HASHING,
    // Bit
    BIT_MANIPULATION,
    // General
    BACKTRACKING,
    UNKNOWN,
};

std::string algorithmName(AlgorithmKind k);
std::string algorithmComplexity(AlgorithmKind k);  // avg time
std::string algorithmBetterAlternative(AlgorithmKind k); // suggestion text

struct PatternCounts {
    std::unordered_map<std::string, int> counts;
    std::vector<int> sortLines, forLines, whileLines, sortCallLines;
    std::set<std::string> functionNames; // for recursion detection

    // Derived flags
    bool hasSort         = false;
    bool hasStlSort      = false;
    bool hasBinSearch    = false;
    bool hasLinearSearch = false;
    bool hasQueue        = false;
    bool hasStack        = false;
    bool hasPriorityQ    = false;
    bool hasHashMap      = false;
    bool hasOrderedMap   = false;
    bool hasMemo         = false;
    bool hasDP           = false;
    bool hasSwap         = false;
    bool hasReverse      = false;
    bool hasBitOp        = false;
    bool hasRecursion    = false;
    bool hasFind         = false;
};

struct Token;

class PatternTracker {
public:
    PatternCounts track(const std::vector<Token>& tokens,
                        const std::vector<std::string>& funcNames);
};
