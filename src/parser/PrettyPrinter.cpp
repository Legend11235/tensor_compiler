#include "tc/parser/PrettyPrinter.h"

namespace tc {

std::string prettyPrint(const Expr& expr) {
    return std::visit([](auto&& node) -> std::string {
        using T = std::decay_t<decltype(node)>;
        if constexpr (std::is_same_v<T, VarNode>) {
            return node.name;
        } else if constexpr (std::is_same_v<T, MatmulNode>) {
            return "matmul(" + prettyPrint(*node.left) + ", " + prettyPrint(*node.right) + ")";
        } else if constexpr (std::is_same_v<T, AddNode>) {
            return "add(" + prettyPrint(*node.left) + ", " + prettyPrint(*node.right) + ")";
        } else if constexpr (std::is_same_v<T, ReluNode>) {
            return "relu(" + prettyPrint(*node.tensor) + ")";
        } else if constexpr (std::is_same_v<T, TransposeNode>) {
            return "transpose(" + prettyPrint(*node.tensor) + ")";
        }
    }, expr.node);
}

}