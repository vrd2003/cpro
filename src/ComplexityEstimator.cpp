#include "ComplexityEstimator.h"

ComplexityResult ComplexityEstimator::estimate(int maxDepth, bool hasSort,
                                                bool hasBinSearch, bool hasMapOp) {
    ComplexityResult r;
    r.depth = maxDepth;

    // Binary search / map operations contribute log n even without sort
    bool logFactor = hasSort || (hasBinSearch) || (hasMapOp);

    if (maxDepth == 0) {
        if (logFactor) {
            r.label = "O(n log n)";
            r.cls   = ComplexityClass::ONLogN;
        } else {
            r.label = "O(1)";
            r.cls   = ComplexityClass::O1;
        }
    } else if (maxDepth == 1) {
        if (hasSort) {
            r.label = "O(n log n)";
            r.cls   = ComplexityClass::ONLogN;
        } else if (hasBinSearch || hasMapOp) {
            r.label = "O(n log n)";
            r.cls   = ComplexityClass::ONLogN;
        } else {
            r.label = "O(n)";
            r.cls   = ComplexityClass::ON;
        }
    } else if (maxDepth == 2) {
        if (hasSort) {
            r.label = "O(n\u00B2 log n)";
            r.cls   = ComplexityClass::ON2LogN;
        } else {
            r.label = "O(n\u00B2)";
            r.cls   = ComplexityClass::ON2;
        }
    } else if (maxDepth == 3) {
        if (hasSort) {
            r.label = "O(n\u00B3 log n)";
            r.cls   = ComplexityClass::ON3;   // lump into ON3 for chart
        } else {
            r.label = "O(n\u00B3)";
            r.cls   = ComplexityClass::ON3;
        }
    } else {
        // depth > 3 — generic O(n^k)
        r.label = "O(n^" + std::to_string(maxDepth) + (hasSort ? " log n)" : ")");
        r.cls   = ComplexityClass::ONK;
    }

    return r;
}
