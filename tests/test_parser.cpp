#include "tc/parser/Lexer.h"
#include "tc/parser/Parser.h"
#include "tc/parser/PrettyPrinter.h"
#include <cassert>
#include <iostream>
#include <string>

void expectRoundTrip(const std::string& src) {
    tc::Lexer lexer(src);
    auto tokens = lexer.tokenize();
    tc::Parser parser(std::move(tokens));
    auto ast = parser.parse();
    std::string out = tc::prettyPrint(*ast);
    if (out != src) {
        std::cerr << "FAIL: expected \"" << src << "\", got \"" << out << "\"\n";
        assert(false);
    } else {
        std::cout << "PASS: " << src << "\n";
    }
}

void expectThrow(const std::string& src, const std::string& label) {
    bool threw = false;
    try {
        tc::Lexer lexer(src);
        auto tokens = lexer.tokenize();
        tc::Parser parser(std::move(tokens));
        parser.parse();
    } catch (const std::exception&) {
        threw = true;
    }
    if (!threw) {
        std::cerr << "FAIL: expected throw for [" << label << "], but it succeeded\n";
        assert(false);
    } else {
        std::cout << "PASS (threw as expected): " << label << "\n";
    }
}

int main() {
    expectRoundTrip("x1");
    expectRoundTrip("matmul(x1, x2)");
    expectRoundTrip("add(x1, x2)");
    expectRoundTrip("relu(x1)");
    expectRoundTrip("transpose(x1)");
    expectRoundTrip("matmul(relu(x1), transpose(x2))");
    expectRoundTrip("add(matmul(x1, x2), relu(transpose(x3)))");

    expectThrow("matmul(x1)", "matmul missing second arg");
    expectThrow("matmul(x1, x2) )", "trailing garbage");
    expectThrow("foo(x1)", "unknown function");
    expectThrow("matmul($)", "invalid character");

    std::cout << "All parser tests passed.\n";
    return 0;
}