#pragma once
#include <stack>
#include <vector>

struct LoopInfo {
    int maxDepth      = 0;
    int totalLoops    = 0;   // for + while + do
    int forLoops      = 0;
    int whileLoops    = 0;
    bool hasSort      = false;
    bool hasBinSearch = false;
    bool hasMapOp     = false;
};

class TokenProcessor;
struct Token;

class LoopDetector {
public:
    LoopInfo detect(const std::vector<Token>& tokens);
private:
    std::stack<int> depthStack; // each entry = depth level at that brace
};
