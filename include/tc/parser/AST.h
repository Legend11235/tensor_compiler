#pragma once
#include <string>
#include <memory>
#include <variant>
#include <vector>
#include <cstdint>


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
    std::vector<int64_t> perm;
};

struct ReshapeNode {
    std::unique_ptr<Expr> tensor;
    std::vector<int64_t> new_shape;
};

struct Location {
    std::string file;
    int line;
    int col;
};

struct Expr {
    Location loc;
    std::variant<VarNode, MatmulNode, ReluNode, AddNode, TransposeNode, ReshapeNode> node;
};

}
