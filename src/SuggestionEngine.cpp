#include "SuggestionEngine.h"
#include "Analyzer.h"
#include "PatternTracker.h"
#include <queue>

std::vector<Suggestion> SuggestionEngine::analyze(const AnalysisResult& res) {
    std::priority_queue<Suggestion> pq;
    int depth = res.maxDepth;
    auto& pc  = res.patterns;

    // ═══════════════════════════════════════════════════════════════════════
    // SECTION 1 — Algorithm Identification & Better Alternative
    // ═══════════════════════════════════════════════════════════════════════
    if (res.primaryAlgorithm != AlgorithmKind::NONE &&
        res.primaryAlgorithm != AlgorithmKind::UNKNOWN) {
        std::string alt = algorithmBetterAlternative(res.primaryAlgorithm);
        if (!alt.empty()) {
            int sev = 3;
            // Severity based on how bad the algorithm is
            if (res.primaryAlgorithm == AlgorithmKind::BUBBLE_SORT ||
                res.primaryAlgorithm == AlgorithmKind::SELECTION_SORT)
                sev = 5;
            else if (res.primaryAlgorithm == AlgorithmKind::INSERTION_SORT ||
                     res.primaryAlgorithm == AlgorithmKind::LINEAR_SEARCH)
                sev = 4;
            else if (res.primaryAlgorithm == AlgorithmKind::BACKTRACKING ||
                     res.primaryAlgorithm == AlgorithmKind::DFS_RECURSIVE)
                sev = 4;
            else
                sev = 2;

            pq.push({sev, "💡",
                "Detected: " + algorithmName(res.primaryAlgorithm) +
                " [" + algorithmComplexity(res.primaryAlgorithm) + "] — " + alt});
        }
    }

    // ═══════════════════════════════════════════════════════════════════════
    // SECTION 2 — Nesting / Complexity Warnings
    // ═══════════════════════════════════════════════════════════════════════
    if (depth >= 3) {
        pq.push({5, "🔴",
            "Triple+ nested loops detected (O(n³)) — "
            "consider breaking the inner logic into helper functions or "
            "use a better algorithm (hash map, two pointers, or DP)."});
    }
    if (depth == 2 && res.hasSort) {
        pq.push({5, "🔴",
            "sort() inside a nested loop gives O(n² log n). "
            "Pre-sort the array once before the outer loop to save O(n) × O(n log n)."});
    }
    if (depth == 2 && !res.hasSort) {
        pq.push({4, "🟠",
            "Nested loops → O(n²). "
            "Consider: two-pointer (O(n) for sorted arrays), "
            "sliding window (O(n) for subarrays), "
            "or hash map (O(n) for lookup problems)."});
    }
    if (depth == 1 && res.hasSort && pc.counts.count("sort") && pc.counts.at("sort") > 0
        && pc.counts.count("for") && pc.counts.at("for") > 0) {
        // sort inside a single loop
        if (!res.patterns.sortLines.empty() && !res.patterns.forLines.empty()) {
            int sortLine = res.patterns.sortLines[0];
            int forLine  = res.patterns.forLines[0];
            if (sortLine > forLine) {
                pq.push({4, "🟠",
                    "sort() appears inside a for-loop — "
                    "if sorting the same container each iteration, "
                    "move sort() before the loop."});
            }
        }
    }

    // ═══════════════════════════════════════════════════════════════════════
    // SECTION 3 — Specific Pattern Suggestions
    // ═══════════════════════════════════════════════════════════════════════

    // Linear search → binary search
    if (res.hasLinSearch && !res.hasBinSearch) {
        pq.push({4, "🟠",
            "Linear search detected (O(n)). "
            "If the data is sorted, use binary_search() / lower_bound() for O(log n). "
            "For repeated lookups, use an unordered_map for O(1) average."});
    }

    // std::find (linear) with no hashing
    if (res.patterns.hasFind && !res.hasHashMap && !res.hasBinSearch) {
        pq.push({3, "🟡",
            "std::find() is O(n). "
            "For repeated element membership tests, prefer an unordered_set (O(1) avg)."});
    }

    // Ordered map performance
    if (res.hasMapOp && !res.hasHashMap) {
        pq.push({3, "🟡",
            "std::map / std::set gives O(log n) per operation. "
            "If order doesn't matter, switch to unordered_map / unordered_set for O(1) avg."});
    }

    // Hash map collision hint
    if (res.hasHashMap) {
        pq.push({1, "🔵",
            "unordered_map detected (O(1) avg). "
            "Call .reserve(n) upfront to avoid rehashing overhead."});
    }

    // BFS / BFS improvement
    if (res.hasQueue && !res.hasPriorityQ && depth >= 1) {
        pq.push({2, "🔵",
            "BFS detected (O(V+E)). "
            "For weighted shortest paths, use Dijkstra with a priority_queue instead."});
    }

    // DFS stack overflow
    if (res.hasRecursion && depth >= 2 && !res.hasQueue) {
        pq.push({3, "🟡",
            "Deep recursion detected — risk of stack overflow for large inputs (n > 10⁴). "
            "Consider converting to iterative DFS with an explicit stack."});
    }

    // Priority queue efficiency
    if (res.hasPriorityQ) {
        pq.push({1, "🔵",
            "priority_queue detected (O(log n) push/pop). "
            "Ensure you're using the right comparator — default is max-heap."});
    }

    // Backtracking pruning
    if (res.hasRecursion && depth >= 2 && res.forLoops >= 2) {
        pq.push({4, "🟠",
            "Potential backtracking detected (exponential complexity). "
            "Apply pruning conditions as early as possible in the recursion tree. "
            "If sub-problems overlap, convert to dynamic programming."});
    }

    // Memoization
    if (res.hasMemo && !res.hasRecursion) {
        pq.push({3, "🟡",
            "DP array detected without clear recursion — "
            "ensure your recurrence relation is correct and all base cases are initialized."});
    }
    if (res.hasMemo && res.hasRecursion) {
        pq.push({2, "🔵",
            "Top-down DP (memoization) detected. "
            "If the number of states is large, consider bottom-up tabulation "
            "to avoid recursion overhead and potential stack overflow."});
    }

    // Two-pointer hint
    if (depth == 2 && res.forLoops >= 2 && !res.hasSort && !res.hasHashMap
        && !res.hasQueue && !res.hasMemo) {
        pq.push({3, "🟡",
            "Two nested for-loops — if this is a pair-sum / two-sum pattern "
            "on a sorted array, the two-pointer technique solves it in O(n)."});
    }

    // Sliding window hint
    if (depth == 1 && res.forLoops >= 1 && !res.hasSort &&
        (res.whileLoops >= 1 || res.forLoops >= 2) && !res.hasHashMap) {
        pq.push({2, "🔵",
            "Possible sliding window pattern — for fixed/variable window subarray "
            "problems, use a two-pointer sliding window for O(n) instead of O(n²)."});
    }

    // Bit manipulation praise/tip
    if (res.hasBitOp) {
        pq.push({1, "🟢",
            "Bit manipulation detected — very efficient (O(1) or O(log n)). "
            "Use __builtin_popcount() for counting set bits instead of a loop."});
    }

    // Already optimal
    if (depth <= 1 && !res.hasSort && res.forLoops <= 1 && !res.hasLinSearch) {
        pq.push({1, "🟢", "Code appears linear O(n) or better — good performance!"});
    }
    if (depth == 0 && res.complexity == "O(1)") {
        pq.push({1, "🟢", "Constant O(1) complexity — optimal!"});
    }

    // Drain into sorted vector
    std::vector<Suggestion> result;
    while (!pq.empty()) {
        result.push_back(pq.top());
        pq.pop();
    }
    return result;
}
