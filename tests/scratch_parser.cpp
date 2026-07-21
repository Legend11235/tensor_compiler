#include "tc/parser/Lexer.h"
#include "tc/parser/Parser.h"
#include <iostream>

void printExpr(const tc::Expr& expr, int indent = 0) {
    std::string pad(static_cast<size_t>(indent), ' ');
    std::visit([&](auto&& node) {
        using T = std::decay_t<decltype(node)>;
        if constexpr (std::is_same_v<T, tc::VarNode>) {
            std::cout << pad << "Var(" << node.name << ")\n";
        } else if constexpr (std::is_same_v<T, tc::MatmulNode>) {
            std::cout << pad << "Matmul\n";
            printExpr(*node.left, indent + 2);
            printExpr(*node.right, indent + 2);
        } else if constexpr (std::is_same_v<T, tc::AddNode>) {
            std::cout << pad << "Add\n";
            printExpr(*node.left, indent + 2);
            printExpr(*node.right, indent + 2);
        } else if constexpr (std::is_same_v<T, tc::ReluNode>) {
            std::cout << pad << "Relu\n";
            printExpr(*node.tensor, indent + 2);
        } else if constexpr (std::is_same_v<T, tc::TransposeNode>) {
            std::cout << pad << "Transpose\n";
            printExpr(*node.tensor, indent + 2);
        }
    }, expr.node);
}

int main() {
    std::string src = "matmul(x1, x2) )";
    tc::Lexer lexer(src);
    auto tokens = lexer.tokenize();

    tc::Parser parser(std::move(tokens));
    auto ast = parser.parse();

    printExpr(*ast);
    return 0;
}