#pragma once
#include <string>
#include <vector>
#include <queue>

struct Suggestion {
    int         severity;  // 1 (info) to 5 (critical)
    std::string icon;
    std::string text;
    bool operator<(const Suggestion& o) const { return severity < o.severity; }
};

// Forward declaration — AnalysisResult is defined in Analyzer.h
struct AnalysisResult;

class SuggestionEngine {
public:
    std::vector<Suggestion> analyze(const AnalysisResult& res);
};
