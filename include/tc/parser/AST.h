#pragma once
#include <string>
#include <memory>
#include <variant>

namespace tc {

struct Expr;

struct VarNode {
    std::string name;
};
struct MatmulNode {
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
};
struct AddNode {
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
};
struct ReluNode {
    std::unique_ptr<Expr> tensor;
};
struct TransposeNode {
    std::unique_ptr<Expr> tensor;
};

struct Location {
    std::string file;
    int line;
    int col;
};

struct Expr {
    Location loc;
    std::variant<VarNode, MatmulNode, ReluNode, AddNode, TransposeNode> node;
};

}
