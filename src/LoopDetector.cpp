#include "LoopDetector.h"
#include "TokenProcessor.h"
#include <stack>

LoopInfo LoopDetector::detect(const std::vector<Token>& tokens) {
    LoopInfo info;
    info.forLoops = info.whileLoops = info.totalLoops = 0;
    info.hasSort = info.hasBinSearch = info.hasMapOp = false;
    info.maxDepth = 0;

    std::stack<bool> loopBraces; // true = this brace was opened by a loop keyword
    int depth = 0;
    bool nextBraceIsLoop = false;

    for (const auto& tok : tokens) {
        switch (tok.type) {
        case TokenType::FOR:
            info.forLoops++;
            info.totalLoops++;
            nextBraceIsLoop = true;
            break;
        case TokenType::WHILE:
            info.whileLoops++;
            info.totalLoops++;
            nextBraceIsLoop = true;
            break;
        case TokenType::DO:
            info.totalLoops++;
            nextBraceIsLoop = true;
            break;
        case TokenType::SORT:
        case TokenType::QSORT:
            info.hasSort = true;
            break;
        case TokenType::BINARY_SEARCH:
            info.hasBinSearch = true;
            break;
        case TokenType::MAP_OP:
        case TokenType::HASH_MAP:
            info.hasMapOp = true;
            break;
        case TokenType::OPEN_BRACE:
            if (nextBraceIsLoop) {
                depth++;
                loopBraces.push(true);
                if (depth > info.maxDepth) info.maxDepth = depth;
                nextBraceIsLoop = false;
            } else {
                loopBraces.push(false);
            }
            break;
        case TokenType::CLOSE_BRACE:
            if (!loopBraces.empty()) {
                if (loopBraces.top()) depth--;
                loopBraces.pop();
            }
            nextBraceIsLoop = false;
            break;
        default:
            nextBraceIsLoop = false;  // any other token resets the pending flag
            break;
        }
    }
    return info;
}
