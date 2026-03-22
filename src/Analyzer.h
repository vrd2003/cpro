#pragma once
#include "ASTNode.h"
#include <string>
#include <memory>

struct AnalysisResult {
    // AST (we no longer compute heuristics here; it's handled by Gemini)
    std::shared_ptr<ASTNode> ast;
};

class Analyzer {
public:
    AnalysisResult analyze(const std::string& code);
};
