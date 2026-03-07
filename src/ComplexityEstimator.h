#pragma once
#include <string>

// Maps to chart curve index in ChartHelper
enum class ComplexityClass {
    O1 = 0,
    OLogN,
    ON,
    ONLogN,
    ON2,
    ON2LogN,
    ON3,
    ONK   // generic O(n^k) for k > 3
};

struct ComplexityResult {
    std::string    label;   // human-readable, e.g. "O(n² log n)"
    ComplexityClass cls;
    int            depth;
};

class ComplexityEstimator {
public:
    ComplexityResult estimate(int maxDepth, bool hasSort,
                              bool hasBinSearch, bool hasMapOp);
};
