#pragma once
#include "tc/parser/AST.h"
#include "tc/parser/Lexer.h"
#include <vector>
#include <memory>

namespace tc {

class Parser {
    public:
    Parser(std::vector<Token> tokens, std::string filename = "<input>");
    std::unique_ptr<Expr> parse();

    private:
    std::vector<Token> tokens_;
    std::string filename_;
    int pos_;

    const Token& peek() const;
    const Token& advance();
    bool check(TokenKind kind) const;
    const Token& expect(TokenKind kind, const std::string& errorMsg);
    bool isAtEnd() const;

    std::unique_ptr<Expr> parseExpr();
};

}