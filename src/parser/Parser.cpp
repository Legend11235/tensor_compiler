#include "tc/parser/Parser.h"
#include <stdexcept>

namespace tc {

Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)), pos_(0) {}

bool Parser::isAtEnd() const {
    return tokens_[static_cast<size_t>(pos_)].kind == TOK_EOF;
}

const Token& Parser::peek() const {
    return tokens_[static_cast<size_t>(pos_)];
}

const Token& Parser::advance() {
    const Token& t = tokens_[static_cast<size_t>(pos_)];
    if (!isAtEnd()) pos_++;
    return t;
}

bool Parser::check(TokenKind kind) const {
    return peek().kind == kind;
}

const Token& Parser::expect(TokenKind kind, const std::string& errorMsg) {
    if (!check(kind)) {
        throw std::runtime_error(
            errorMsg + " at line " + std::to_string(peek().line) +
            " (got '" + peek().lexeme + "')");
    }
    return advance();
}

std::unique_ptr<Expr> Parser::parseExpr() {
    Token nameTok = expect(IDENT, "Expected identifier");
    std::string name = nameTok.lexeme;

    // Bare variable: identifier not followed by '('
    if (!check(LPAREN)) {
        return std::make_unique<Expr>(Expr{VarNode{name}});
    }

    // Function-call form: IDENT '(' ... ')'
    expect(LPAREN, "Expected '(' after '" + name + "'");

    if (name == "matmul" || name == "add") {
        auto left = parseExpr();
        expect(COMMA, "Expected ',' between arguments of '" + name + "'");
        auto right = parseExpr();
        expect(RPAREN, "Expected ')' to close '" + name + "'");

        if (name == "matmul") {
            return std::make_unique<Expr>(Expr{MatmulNode{std::move(left), std::move(right)}});
        } else {
            return std::make_unique<Expr>(Expr{AddNode{std::move(left), std::move(right)}});
        }

    } else if (name == "relu" || name == "transpose") {
        auto arg = parseExpr();
        expect(RPAREN, "Expected ')' to close '" + name + "'");

        if (name == "relu") {
            return std::make_unique<Expr>(Expr{ReluNode{std::move(arg)}});
        } else {
            return std::make_unique<Expr>(Expr{TransposeNode{std::move(arg)}});
        }

    } else {
        throw std::runtime_error(
            "Unknown function '" + name + "' at line " + std::to_string(nameTok.line));
    }
}

std::unique_ptr<Expr> Parser::parse() {
    auto result = parseExpr();
    if (!isAtEnd()) {
        throw std::runtime_error(
            "Unexpected trailing tokens after expression, starting at line " +
            std::to_string(peek().line) + " (got '" + peek().lexeme + "')");
    }
    return result;
}

}