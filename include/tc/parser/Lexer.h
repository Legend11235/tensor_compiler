#pragma once
#include <string>
#include <vector>

namespace tc {

    enum TokenKind{
        LPAREN, RPAREN, COMMA, IDENT, TOK_EOF
    };

    struct Token{
        TokenKind kind;
        std::string lexeme;
        int line;
    };

    class Lexer {
        public:
        Lexer(const std::string& source);
        std::vector<Token> tokenize() ;

        private: 
        std::string source_;
        int pos_;
        int line_;
        char peek() const;
        char advance();
        bool isAtEnd() const;
        void skipWhiteSpace();
        Token scanIdentifier();
        Token makeToken(TokenKind kind, const std::string& lexeme, int line);
    };

}
