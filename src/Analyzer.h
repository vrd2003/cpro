#pragma once
#include "ASTNode.h"
#include "ComplexityEstimator.h"
#include "SuggestionEngine.h"
#include "LoopDetector.h"
#include "PatternTracker.h"
#include <string>
#include <memory>
#include <vector>

struct AnalysisResult {
    // Complexity
    std::string       complexity;
    ComplexityClass   complexityClass;
    int               maxDepth   = 0;

    // Raw flags
    bool hasSort      = false;
    bool hasBinSearch = false;
    bool hasMapOp     = false;
    bool hasHashMap   = false;
    bool hasMemo      = false;
    bool hasQueue     = false;
    bool hasStack     = false;
    bool hasPriorityQ = false;
    bool hasRecursion = false;
    bool hasBitOp     = false;
    bool hasLinSearch = false;
    bool hasSwap      = false;

    // Loop counts
    int forLoops  = 0;
    int whileLoops= 0;
    int totalLoops= 0;

    // Identified algorithm(s) — primary and secondary
    AlgorithmKind     primaryAlgorithm   = AlgorithmKind::NONE;
    AlgorithmKind     secondaryAlgorithm = AlgorithmKind::NONE;

    // Full pattern tracker result (for suggestions)
    PatternCounts     patterns;

    // AST and suggestions
    std::shared_ptr<ASTNode>  ast;
    std::vector<Suggestion>   suggestions;
};

class Analyzer {
public:
    AnalysisResult analyze(const std::string& code);
private:
    AlgorithmKind identifyPrimary(const AnalysisResult& partial) const;
    AlgorithmKind identifySecondary(const AnalysisResult& partial) const;
};
