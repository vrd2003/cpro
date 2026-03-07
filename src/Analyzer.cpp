#include "Analyzer.h"
#include "TokenProcessor.h"
#include "LoopDetector.h"
#include "ASTBuilder.h"
#include "PatternTracker.h"
#include "ComplexityEstimator.h"
#include "SuggestionEngine.h"

using namespace std;

AnalysisResult Analyzer::analyze(const string& code) {
    AnalysisResult result;

    // 1. Tokenize
    TokenProcessor tp;
    auto tokens = tp.tokenize(code);

    // 2. Loop detection
    LoopDetector ld;
    LoopInfo loopInfo = ld.detect(tokens);

    // 3. Build AST
    ASTBuilder ab;
    result.ast = ab.build(tokens);

    // 4. Pattern counting
    PatternTracker pt;
    result.patterns = pt.track(tokens, tp.functionNames);

    // 5. Estimate complexity
    ComplexityEstimator ce;
    auto cr = ce.estimate(loopInfo.maxDepth, loopInfo.hasSort,
                          loopInfo.hasBinSearch, loopInfo.hasMapOp);
    result.complexity      = cr.label;
    result.complexityClass = cr.cls;
    result.maxDepth        = loopInfo.maxDepth;

    // 6. Copy flags from patterns
    result.hasSort      = result.patterns.hasSort;
    result.hasBinSearch = result.patterns.hasBinSearch;
    result.hasMapOp     = result.patterns.hasOrderedMap;
    result.hasHashMap   = result.patterns.hasHashMap;
    result.hasMemo      = result.patterns.hasMemo;
    result.hasQueue     = result.patterns.hasQueue;
    result.hasStack     = result.patterns.hasStack;
    result.hasPriorityQ = result.patterns.hasPriorityQ;
    result.hasRecursion = result.patterns.hasRecursion;
    result.hasBitOp     = result.patterns.hasBitOp;
    result.hasLinSearch = result.patterns.hasLinearSearch;
    result.hasSwap      = result.patterns.hasSwap;
    result.forLoops     = loopInfo.forLoops;
    result.whileLoops   = loopInfo.whileLoops;
    result.totalLoops   = loopInfo.totalLoops;

    // 7. Identify algorithm paradigms
    result.primaryAlgorithm   = identifyPrimary(result);
    result.secondaryAlgorithm = identifySecondary(result);

    // 8. Generate suggestions
    SuggestionEngine se;
    result.suggestions = se.analyze(result);

    return result;
}

AlgorithmKind Analyzer::identifyPrimary(const AnalysisResult& r) const {
    int depth = r.maxDepth;
    auto& pc  = r.patterns;

    // ── DP / Memoization ─────────────────────────────────────────────────
    if (r.hasMemo && r.hasRecursion)    return AlgorithmKind::MEMOIZATION;
    if (r.hasMemo && depth >= 1)        return AlgorithmKind::TABULATION;

    // ── Graph traversal ──────────────────────────────────────────────────
    if (r.hasQueue && depth >= 1)       return AlgorithmKind::BFS;
    if (r.hasStack && depth >= 1 && r.hasRecursion) return AlgorithmKind::DFS_RECURSIVE;
    if (r.hasStack && depth >= 1)       return AlgorithmKind::DFS_ITERATIVE;

    // ── Greedy ───────────────────────────────────────────────────────────
    if (r.hasPriorityQ)                 return AlgorithmKind::GREEDY_HEAP;

    // ── Sorting algorithms (manual) ──────────────────────────────────────
    // Bubble sort: 2 nested loops + swap
    if (depth == 2 && r.hasSwap && !r.hasSort)
        return AlgorithmKind::BUBBLE_SORT;
    // Selection sort: 2 nested loops, no swap on inner (hard to distinguish), fallback
    if (depth == 2 && !r.hasSwap && !r.hasSort && pc.counts.at("for") >= 2)
        return AlgorithmKind::SELECTION_SORT;
    // Insertion sort: 1 while + shift pattern
    if (depth == 1 && r.whileLoops >= 1 && !r.hasSort)
        return AlgorithmKind::INSERTION_SORT;

    // ── Quick/Merge sort (recursive) ─────────────────────────────────────
    if (r.hasRecursion && depth >= 1 && r.hasSort)
        return AlgorithmKind::STL_SORT;
    if (r.hasRecursion && depth >= 1 && r.hasSwap)
        return AlgorithmKind::QUICK_SORT;
    if (r.hasRecursion && depth >= 1)
        return AlgorithmKind::DIVIDE_AND_CONQUER;

    // ── STL sort ─────────────────────────────────────────────────────────
    if (r.hasSort)                       return AlgorithmKind::STL_SORT;

    // ── Searching ────────────────────────────────────────────────────────
    if (r.hasBinSearch)                 return AlgorithmKind::BINARY_SEARCH;
    if (r.hasLinSearch && depth == 1)   return AlgorithmKind::LINEAR_SEARCH;

    // ── Two pointer / sliding window ─────────────────────────────────────
    if (depth == 1 && r.forLoops >= 1 && !r.hasSort && !r.hasQueue)
        return AlgorithmKind::LINEAR_SEARCH; // generic linear

    // ── Hashing ──────────────────────────────────────────────────────────
    if (r.hasHashMap)                   return AlgorithmKind::HASHING;

    // ── Bit manipulation ─────────────────────────────────────────────────
    if (r.hasBitOp)                     return AlgorithmKind::BIT_MANIPULATION;

    // ── Backtracking: deep recursion with many branches ──────────────────
    if (r.hasRecursion && depth >= 2)   return AlgorithmKind::BACKTRACKING;

    return AlgorithmKind::UNKNOWN;
}

AlgorithmKind Analyzer::identifySecondary(const AnalysisResult& r) const {
    // Give a secondary classification based on other flags
    if (r.hasHashMap && r.primaryAlgorithm != AlgorithmKind::HASHING)
        return AlgorithmKind::HASHING;
    if (r.hasBinSearch && r.primaryAlgorithm != AlgorithmKind::BINARY_SEARCH)
        return AlgorithmKind::BINARY_SEARCH;
    if (r.hasBitOp && r.primaryAlgorithm != AlgorithmKind::BIT_MANIPULATION)
        return AlgorithmKind::BIT_MANIPULATION;
    if (r.hasPriorityQ && r.primaryAlgorithm != AlgorithmKind::GREEDY_HEAP)
        return AlgorithmKind::GREEDY_HEAP;
    if (r.hasSort && r.primaryAlgorithm != AlgorithmKind::STL_SORT)
        return AlgorithmKind::STL_SORT;
    return AlgorithmKind::NONE;
}
