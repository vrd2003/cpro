#pragma once
#include "ASTNode.h"
#include "TokenProcessor.h"
#include <memory>
#include <vector>

class ASTBuilder {
public:
    // Build an AST from the flat token list; returns the root "Program" node
    std::shared_ptr<ASTNode> build(const std::vector<Token>& tokens);
};
