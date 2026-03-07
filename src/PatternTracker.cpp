#include "PatternTracker.h"
#include "TokenProcessor.h"
#include <algorithm>

// ── Algorithm name / complexity / alternative lookup ──────────────────────
std::string algorithmName(AlgorithmKind k) {
    switch(k) {
    case AlgorithmKind::BUBBLE_SORT:        return "Bubble Sort";
    case AlgorithmKind::SELECTION_SORT:     return "Selection Sort";
    case AlgorithmKind::INSERTION_SORT:     return "Insertion Sort";
    case AlgorithmKind::MERGE_SORT:         return "Merge Sort";
    case AlgorithmKind::QUICK_SORT:         return "Quick Sort";
    case AlgorithmKind::HEAP_SORT:          return "Heap Sort";
    case AlgorithmKind::STL_SORT:           return "std::sort (Introsort)";
    case AlgorithmKind::COUNTING_SORT:      return "Counting/Frequency Array";
    case AlgorithmKind::LINEAR_SEARCH:      return "Linear Search";
    case AlgorithmKind::BINARY_SEARCH:      return "Binary Search";
    case AlgorithmKind::BFS:                return "BFS (Breadth-First Search)";
    case AlgorithmKind::DFS_RECURSIVE:      return "DFS (Recursive)";
    case AlgorithmKind::DFS_ITERATIVE:      return "DFS (Iterative / Stack)";
    case AlgorithmKind::MEMOIZATION:        return "Top-Down DP (Memoization)";
    case AlgorithmKind::TABULATION:         return "Bottom-Up DP (Tabulation)";
    case AlgorithmKind::GREEDY_HEAP:        return "Greedy with Priority Queue";
    case AlgorithmKind::DIVIDE_AND_CONQUER: return "Divide and Conquer";
    case AlgorithmKind::TWO_POINTER:        return "Two-Pointer Technique";
    case AlgorithmKind::SLIDING_WINDOW:     return "Sliding Window";
    case AlgorithmKind::HASHING:            return "Hash Table / Hashing";
    case AlgorithmKind::BIT_MANIPULATION:   return "Bit Manipulation";
    case AlgorithmKind::BACKTRACKING:       return "Backtracking";
    default:                                return "Unknown";
    }
}

std::string algorithmComplexity(AlgorithmKind k) {
    switch(k) {
    case AlgorithmKind::BUBBLE_SORT:        return "O(n²) avg, O(n²) worst";
    case AlgorithmKind::SELECTION_SORT:     return "O(n²) always";
    case AlgorithmKind::INSERTION_SORT:     return "O(n²) avg, O(n) best";
    case AlgorithmKind::MERGE_SORT:         return "O(n log n) always";
    case AlgorithmKind::QUICK_SORT:         return "O(n log n) avg, O(n²) worst";
    case AlgorithmKind::HEAP_SORT:          return "O(n log n) always";
    case AlgorithmKind::STL_SORT:           return "O(n log n) guaranteed";
    case AlgorithmKind::COUNTING_SORT:      return "O(n + k)";
    case AlgorithmKind::LINEAR_SEARCH:      return "O(n)";
    case AlgorithmKind::BINARY_SEARCH:      return "O(log n)";
    case AlgorithmKind::BFS:                return "O(V + E)";
    case AlgorithmKind::DFS_RECURSIVE:      return "O(V + E)";
    case AlgorithmKind::DFS_ITERATIVE:      return "O(V + E)";
    case AlgorithmKind::MEMOIZATION:        return "Varies (subproblem count)";
    case AlgorithmKind::TABULATION:         return "Varies (table dimensions)";
    case AlgorithmKind::GREEDY_HEAP:        return "O(n log n)";
    case AlgorithmKind::DIVIDE_AND_CONQUER: return "O(n log n) typical";
    case AlgorithmKind::TWO_POINTER:        return "O(n)";
    case AlgorithmKind::SLIDING_WINDOW:     return "O(n)";
    case AlgorithmKind::HASHING:            return "O(1) avg per lookup";
    case AlgorithmKind::BIT_MANIPULATION:   return "O(1) or O(log n)";
    case AlgorithmKind::BACKTRACKING:       return "O(b^d) — exponential";
    default:                                return "Unknown";
    }
}

std::string algorithmBetterAlternative(AlgorithmKind k) {
    switch(k) {
    case AlgorithmKind::BUBBLE_SORT:
        return "Replace with std::sort() — O(n log n) introsort. "
               "Or use Insertion Sort for nearly-sorted small arrays.";
    case AlgorithmKind::SELECTION_SORT:
        return "Replace with std::sort() (O(n log n)) or Heap Sort. "
               "Selection Sort does n² comparisons regardless of input.";
    case AlgorithmKind::INSERTION_SORT:
        return "Good for small/nearly-sorted arrays (<32 elements). "
               "For larger inputs use std::sort() or Merge Sort.";
    case AlgorithmKind::LINEAR_SEARCH:
        return "If the array is sorted, use binary_search() / lower_bound() "
               "for O(log n). For repeated lookups use an unordered_map (O(1)).";
    case AlgorithmKind::QUICK_SORT:
        return "std::sort() uses Introsort (QuickSort + HeapSort fallback) "
               "guaranteeing O(n log n). Avoid manual QuickSort in competitions.";
    case AlgorithmKind::BFS:
        return "BFS is optimal for unweighted shortest path. "
               "For weighted graphs use Dijkstra with priority_queue.";
    case AlgorithmKind::DFS_RECURSIVE:
        return "Recursive DFS may cause stack overflow on deep graphs (n>10⁵). "
               "Consider iterative DFS with an explicit stack.";
    case AlgorithmKind::MEMOIZATION:
        return "Good approach. Ensure your state is fully captured in the key. "
               "If recursion depth is large, convert to bottom-up tabulation.";
    case AlgorithmKind::TABULATION:
        return "Optimal DP approach. Consider space optimization: "
               "if each row only depends on the previous, use a 1D rolling array.";
    case AlgorithmKind::GREEDY_HEAP:
        return "Greedy with priority_queue is correct for problems like "
               "Dijkstra, Prim's MST, Huffman coding. Verify greedy choice property.";
    case AlgorithmKind::TWO_POINTER:
        return "Efficient O(n) approach. Ensure the array is sorted beforehand "
               "if the two-pointer relies on order.";
    case AlgorithmKind::SLIDING_WINDOW:
        return "Optimal O(n) for fixed or variable window problems. "
               "Use a deque for maximum in window (monotonic deque).";
    case AlgorithmKind::HASHING:
        return "unordered_map gives O(1) avg lookups. "
               "Use reserve() to avoid rehashing: umap.reserve(n).";
    case AlgorithmKind::BIT_MANIPULATION:
        return "Efficient for subset enumeration and parity checks. "
               "Consider __builtin_popcount() for bit counting instead of loops.";
    case AlgorithmKind::BACKTRACKING:
        return "Exponential complexity — apply pruning early. "
               "Consider memoization if sub-states repeat (→ DP).";
    case AlgorithmKind::DIVIDE_AND_CONQUER:
        return "Typically O(n log n). Use iterative solutions when "
               "recursion depth is large to avoid stack overhead.";
    default:
        return "";
    }
}

PatternCounts PatternTracker::track(const std::vector<Token>& tokens,
                                    const std::vector<std::string>& funcNames) {
    PatternCounts pc;
    pc.counts["for"] = pc.counts["while"] = pc.counts["do"] = 0;
    pc.counts["if"]  = pc.counts["sort"]  = pc.counts["bsearch"] = 0;
    pc.counts["queue"] = pc.counts["stack"] = pc.counts["pq"] = 0;
    pc.counts["hashmap"] = pc.counts["omap"] = pc.counts["memo"] = 0;
    pc.counts["swap"] = pc.counts["reverse"] = pc.counts["bit"] = 0;
    pc.counts["find"] = pc.counts["func"] = 0;

    for (const auto& fn : funcNames) pc.functionNames.insert(fn);

    for (const auto& tok : tokens) {
        switch (tok.type) {
        case TokenType::FOR:
            pc.counts["for"]++; pc.forLines.push_back(tok.lineNumber); break;
        case TokenType::WHILE:
            pc.counts["while"]++; pc.whileLines.push_back(tok.lineNumber); break;
        case TokenType::DO:
            pc.counts["do"]++; break;
        case TokenType::SORT:
        case TokenType::QSORT:
            pc.counts["sort"]++; pc.hasSort = true; pc.hasStlSort = true;
            pc.sortLines.push_back(tok.lineNumber); break;
        case TokenType::BINARY_SEARCH:
            pc.counts["bsearch"]++; pc.hasBinSearch = true; break;
        case TokenType::FIND:
            pc.counts["find"]++; pc.hasFind = true; pc.hasLinearSearch = true; break;
        case TokenType::QUEUE_OP:
        case TokenType::DEQUE_OP:
            pc.counts["queue"]++; pc.hasQueue = true; break;
        case TokenType::STACK_OP:
            pc.counts["stack"]++; pc.hasStack = true; break;
        case TokenType::PRIORITY_QUEUE:
        case TokenType::PUSH_HEAP:
            pc.counts["pq"]++; pc.hasPriorityQ = true; break;
        case TokenType::HASH_MAP:
            pc.counts["hashmap"]++; pc.hasHashMap = true; break;
        case TokenType::MAP_OP:
            pc.counts["omap"]++; pc.hasOrderedMap = true; break;
        case TokenType::MEMO_MAP:
            pc.counts["memo"]++; pc.hasMemo = true; pc.hasDP = true; break;
        case TokenType::SWAP_OP:
            pc.counts["swap"]++; pc.hasSwap = true; break;
        case TokenType::REVERSE_OP:
            pc.counts["reverse"]++; pc.hasReverse = true; break;
        case TokenType::BIT_OP:
            pc.counts["bit"]++; pc.hasBitOp = true; break;
        case TokenType::FUNC_DEF:
            pc.counts["func"]++; break;
        default: break;
        }
    }

    // Recursion: check if any function name appears as a FUNC_DEF token text
    // (self-call in the token stream)
    // simple heuristic: if funcNames has >0 names and any token text matches
    for (const auto& tok : tokens) {
        if (tok.type == TokenType::FUNC_DEF &&
            pc.functionNames.count(tok.text) > 0)
            pc.hasRecursion = true;
    }

    return pc;
}
