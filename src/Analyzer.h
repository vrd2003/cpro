#pragma once
#include "ASTNode.h"
#include <string>
#include <memory>

struct AnalysisResult {
    // AST produced by ASTBuilder; complexity analysis is handled by GeminiClient
    std::shared_ptr<ASTNode> ast;
};

class Analyzer {
public:
    AnalysisResult analyze(const std::string& code);
};
