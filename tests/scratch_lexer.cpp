#include "tc/parser/Lexer.h"
#include <iostream>

int main() {
    tc::Lexer lexer("matmul($)");
    auto tokens = lexer.tokenize();
    for (const auto& t : tokens) {
        std::cout << "kind=" << t.kind << " lexeme=\"" << t.lexeme << "\" line=" << t.line << "\n";
    }
    return 0;
}