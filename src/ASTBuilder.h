#pragma once
#include "ASTNode.h"
#include "TokenProcessor.h"
#include <memory>
#include <vector>

class ASTBuilder {
public:
    // Build an AST from the flat token list; returns the root "Program" node
    std::shared_ptr<ASTNode> build(const std::vector<Token>& tokens);

private:
    // Allocates a new ASTNode — extracted from the per-iteration lambda for
    // testability and to avoid repeated lambda construction in the hot loop.
    std::shared_ptr<ASTNode> makeNode(const std::string& label,
                                      int depth, int lineNum) const;
};
