#include "tc/parser/Lexer.h"
#include <stdexcept>

namespace tc{
    
Lexer::Lexer(const std::string& source){
    source_ = source;
    pos_ = 0;
    line_ = 1;
}

bool Lexer::isAtEnd() const {
    return pos_ >= static_cast<int>(source_.size()); // static cast converts value into into
}

// look at current character without consuming it
char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source_[static_cast<size_t>(pos_)];
}

// consume current character and move to next character
char Lexer::advance() {
    char c = source_[static_cast<size_t>(pos_)];
    pos_++;
    return c;
}

// skips white spaces and also keeps track of line number
void Lexer::skipWhiteSpace() {
    while(!isAtEnd()) {
        char c = peek();
        if(c == '\n'){
            line_++;
            advance();
        } else if (std::isspace(static_cast<unsigned char>(c))) {
            advance();
        } else{
            break;
        }
    }
}

// consumes a string and returns it as an IDENT token
Token Lexer::scanIdentifier() {
    int startLine = line_;
    std::string lexeme;
    while(!isAtEnd() && (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_')){
        lexeme += advance();
    }
    return Token{IDENT, lexeme, startLine};
}

// Small convenience wrapper for building a Token so tokenize() doesn't
Token Lexer::makeToken(TokenKind kind, const std::string& lexeme, int line){
    return Token{kind, lexeme, line};
}

// main scanning loop
std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        skipWhiteSpace();
        if (isAtEnd()) break;

        int startLine = line_;
        char c = peek();

        if (c == '(') {
            advance();
            tokens.push_back(makeToken(LPAREN, "(", startLine));
        } else if (c == ')') {
            advance();
            tokens.push_back(makeToken(RPAREN, ")", startLine));
        } else if (c == ',') {
            advance();
            tokens.push_back(makeToken(COMMA, ",", startLine));
        } else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            tokens.push_back(scanIdentifier());
        } else {
            throw std::runtime_error(
                "Unexpected character '" + std::string(1, c) + "' at line " + std::to_string(startLine));
        }
    }

    tokens.push_back(makeToken(TOK_EOF, "", line_));
    return tokens;
}

}  