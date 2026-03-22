#pragma once
#include "ASTNode.h"
#include "TokenProcessor.h"
#include <memory>
#include <vector>

using namespace std;

class ASTBuilder {
public:
    // Build an AST from the flat token list; returns the root "Program" node
    shared_ptr<ASTNode> build(const vector<Token>& tokens);
};
