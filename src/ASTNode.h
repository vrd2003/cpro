#pragma once
#include <string>
#include <vector>
#include <memory>

// Every node in the Abstract Syntax Tree
struct ASTNode {
    std::string              label;      // "Program", "for", "while", "sort()", "if", "func"
    int                      depth;      // nesting depth (0 = root)
    int                      lineNumber; // source line where this construct starts
    std::vector<std::shared_ptr<ASTNode>> children;

    explicit ASTNode(const std::string& lbl = "Program",
                     int dep = 0,
                     int line = 0)
        : label(lbl), depth(dep), lineNumber(line) {}
};
