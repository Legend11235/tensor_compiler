#include "tc/parser/Parser.h"
#include <stdexcept>

namespace tc {

Parser::Parser(std::vector<Token> tokens, std::string filename)
    : tokens_(std::move(tokens)), filename_(std::move(filename)), pos_(0) {}

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
    Location loc{filename_, nameTok.line, nameTok.col};
    std::string name = nameTok.lexeme;

    if (!check(LPAREN)) {
        return std::make_unique<Expr>(Expr{loc, VarNode{name}});
    }

    expect(LPAREN, "Expected '(' after '" + name + "'");

    if (name == "matmul" || name == "add") {
        auto left = parseExpr();
        expect(COMMA, "Expected ',' between arguments of '" + name + "'");
        auto right = parseExpr();
        expect(RPAREN, "Expected ')' to close '" + name + "'");

        if (name == "matmul") {
            return std::make_unique<Expr>(Expr{loc, MatmulNode{std::move(left), std::move(right)}});
        } else {
            return std::make_unique<Expr>(Expr{loc, AddNode{std::move(left), std::move(right)}});
        }

        } else if (name == "relu") {
        auto arg = parseExpr();
        expect(RPAREN, "Expected ')' to close '" + name + "'");
        return std::make_unique<Expr>(Expr{loc, ReluNode{std::move(arg)}});

    } else if (name == "transpose" || name == "reshape") {
        auto arg = parseExpr();
        expect(COMMA, "Expected ',' before list in '" + name + "'");
        std::vector<int64_t> list = parseIntList();
        expect(RPAREN, "Expected ')' to close '" + name + "'");

        if (name == "transpose") {
        return std::make_unique<Expr>(Expr{loc, TransposeNode{std::move(arg), std::move(list)}});
    } else {
        return std::make_unique<Expr>(Expr{loc, ReshapeNode{std::move(arg), std::move(list)}});
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

std::vector<int64_t> Parser::parseIntList() {
    std::vector<int64_t> values;
    expect(LBRACKET, "Expected '[' to start list");

    // atleast one number to enter in this if statement
    if (!check(RBRACKET)) {
        const Token& first = expect(INT_LIT, "Expected integer in list");
        values.push_back(std::stoll(first.lexeme));

        while (check(COMMA)) {
            advance();
            const Token& next = expect(INT_LIT, "Expected integer in list");
            values.push_back(std::stoll(next.lexeme));
        }
    }

    expect(RBRACKET, "Expected ']' to close list");
    return values;
}


}