#include "ASTBuilder.h"
#include "TokenProcessor.h"
#include <stack>

std::shared_ptr<ASTNode> ASTBuilder::build(const std::vector<Token>& tokens) {
    auto root = std::make_shared<ASTNode>("Program", 0, 0);

    // Parent stack: top = current parent node
    std::stack<std::shared_ptr<ASTNode>> parentStack;
    parentStack.push(root);

    // pending keyword to attach when we see the next open brace
    std::shared_ptr<ASTNode> pendingNode = nullptr;
    bool nextBraceIsControl = false;

    for (const auto& tok : tokens) {
        auto makeNode = [&](const std::string& label) {
            int depth = (int)parentStack.size() - 1;
            return std::make_shared<ASTNode>(label, depth, tok.lineNumber);
        };

        switch (tok.type) {
        case TokenType::FOR: {
            pendingNode = makeNode("for");
            nextBraceIsControl = true;
            break;
        }
        case TokenType::WHILE: {
            pendingNode = makeNode("while");
            nextBraceIsControl = true;
            break;
        }
        case TokenType::DO: {
            pendingNode = makeNode("do");
            nextBraceIsControl = true;
            break;
        }
        case TokenType::IF: {
            pendingNode = makeNode("if");
            nextBraceIsControl = true;
            break;
        }
        case TokenType::ELSE: {
            pendingNode = makeNode("else");
            nextBraceIsControl = true;
            break;
        }
        case TokenType::SORT: {
            // Leaf node — directly attach to current parent
            auto node = makeNode("sort()");
            parentStack.top()->children.push_back(node);
            break;
        }
        case TokenType::BINARY_SEARCH: {
            auto node = makeNode(tok.text + "()");
            parentStack.top()->children.push_back(node);
            break;
        }
        case TokenType::HASH_MAP:
        case TokenType::MAP_OP: {
            auto node = makeNode(tok.text);
            parentStack.top()->children.push_back(node);
            break;
        }
        case TokenType::QUEUE_OP:
        case TokenType::DEQUE_OP: {
            auto node = makeNode("queue<" + tok.text + ">");
            parentStack.top()->children.push_back(node);
            break;
        }
        case TokenType::STACK_OP: {
            auto node = makeNode("stack<T>");
            parentStack.top()->children.push_back(node);
            break;
        }
        case TokenType::PRIORITY_QUEUE:
        case TokenType::PUSH_HEAP: {
            auto node = makeNode("priority_queue");
            parentStack.top()->children.push_back(node);
            break;
        }
        case TokenType::MEMO_MAP: {
            auto node = makeNode("dp/memo[]");
            parentStack.top()->children.push_back(node);
            break;
        }
        case TokenType::SWAP_OP: {
            auto node = makeNode("swap()");
            parentStack.top()->children.push_back(node);
            break;
        }
        case TokenType::BIT_OP: {
            auto node = makeNode("bit-op");
            parentStack.top()->children.push_back(node);
            break;
        }
        case TokenType::FUNC_DEF: {
            if (tok.text != "main" && tok.text.size() > 1) {
                auto node = makeNode(tok.text + "()");
                parentStack.top()->children.push_back(node);
            }
            break;
        }
        case TokenType::OPEN_BRACE: {
            if (nextBraceIsControl && pendingNode) {
                // Attach to current parent, then push as new parent
                parentStack.top()->children.push_back(pendingNode);
                parentStack.push(pendingNode);
                pendingNode = nullptr;
            }
            nextBraceIsControl = false;
            break;
        }
        case TokenType::CLOSE_BRACE: {
            if (parentStack.size() > 1) {
                parentStack.pop();
            }
            pendingNode = nullptr;
            nextBraceIsControl = false;
            break;
        }
        default:
            break;
        }
    }

    return root;
}
