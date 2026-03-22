#include "Analyzer.h"
#include "TokenProcessor.h"
#include "ASTBuilder.h"

using namespace std;

AnalysisResult Analyzer::analyze(const string& code) {
    AnalysisResult result;

    // 1. Tokenize
    TokenProcessor tp;
    auto tokens = tp.tokenize(code);

    // 2. Build AST
    ASTBuilder ab;
    result.ast = ab.build(tokens);

    return result;
}
