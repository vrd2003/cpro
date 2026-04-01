#include "ASTBuilder.h"
#include "TokenProcessor.h"
#include <stack>

using namespace std;

// ── Private helper ────────────────────────────────────────────────────────────

shared_ptr<ASTNode> ASTBuilder::makeNode(const string& label,
                                          int depth, int lineNum) const {
    return make_shared<ASTNode>(label, depth, lineNum);
}

// ── build ─────────────────────────────────────────────────────────────────────

shared_ptr<ASTNode> ASTBuilder::build(const vector<Token>& tokens) {
    auto root = make_shared<ASTNode>("Program", 0, 0);

    // Parent stack: top = current parent node
    stack<shared_ptr<ASTNode>> parentStack;
    parentStack.push(root);

    shared_ptr<ASTNode> pendingNode    = nullptr;
    bool                nextBraceIsControl = false;

    for (const auto& tok : tokens) {
        const int depth = static_cast<int>(parentStack.size()) - 1;

        switch (tok.type) {

        case TokenType::FOR:
            pendingNode = makeNode("for",   depth, tok.lineNumber);
            nextBraceIsControl = true;
            break;

        case TokenType::WHILE:
            pendingNode = makeNode("while", depth, tok.lineNumber);
            nextBraceIsControl = true;
            break;

        case TokenType::DO:
            pendingNode = makeNode("do",    depth, tok.lineNumber);
            nextBraceIsControl = true;
            break;

        case TokenType::IF:
            pendingNode = makeNode("if",    depth, tok.lineNumber);
            nextBraceIsControl = true;
            break;

        case TokenType::ELSE:
            pendingNode = makeNode("else",  depth, tok.lineNumber);
            nextBraceIsControl = true;
            break;

        case TokenType::SORT:
            parentStack.top()->children.push_back(
                makeNode("sort()", depth, tok.lineNumber));
            break;

        case TokenType::BINARY_SEARCH:
            parentStack.top()->children.push_back(
                makeNode(tok.text + "()", depth, tok.lineNumber));
            break;

        case TokenType::HASH_MAP:
        case TokenType::MAP_OP:
            parentStack.top()->children.push_back(
                makeNode(tok.text, depth, tok.lineNumber));
            break;

        case TokenType::QUEUE_OP:
        case TokenType::DEQUE_OP:
            parentStack.top()->children.push_back(
                makeNode("queue<" + tok.text + ">", depth, tok.lineNumber));
            break;

        case TokenType::STACK_OP:
            parentStack.top()->children.push_back(
                makeNode("stack<T>", depth, tok.lineNumber));
            break;

        case TokenType::PRIORITY_QUEUE:
        case TokenType::PUSH_HEAP:
            parentStack.top()->children.push_back(
                makeNode("priority_queue", depth, tok.lineNumber));
            break;

        case TokenType::MEMO_MAP:
            parentStack.top()->children.push_back(
                makeNode("dp/memo[]", depth, tok.lineNumber));
            break;

        case TokenType::SWAP_OP:
            parentStack.top()->children.push_back(
                makeNode("swap()", depth, tok.lineNumber));
            break;

        case TokenType::BIT_OP:
            parentStack.top()->children.push_back(
                makeNode("bit-op", depth, tok.lineNumber));
            break;

        case TokenType::FUNC_DEF:
            if (tok.text != "main" && tok.text.size() > 1) {
                parentStack.top()->children.push_back(
                    makeNode(tok.text + "()", depth, tok.lineNumber));
            }
            break;

        case TokenType::OPEN_BRACE:
            if (nextBraceIsControl && pendingNode) {
                parentStack.top()->children.push_back(pendingNode);
                parentStack.push(pendingNode);
                pendingNode = nullptr;
            }
            nextBraceIsControl = false;
            break;

        case TokenType::CLOSE_BRACE:
            if (parentStack.size() > 1) parentStack.pop();
            pendingNode        = nullptr;
            nextBraceIsControl = false;
            break;

        default:
            break;
        }
    }

    return root;
}
