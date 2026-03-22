#pragma once
#include "ASTNode.h"
#include <string>
#include <memory>

using namespace std;

struct AnalysisResult {
    // AST (we no longer compute heuristics here; it's handled by Gemini)
    shared_ptr<ASTNode> ast;
};

class Analyzer {
public:
    AnalysisResult analyze(const string& code);
};
