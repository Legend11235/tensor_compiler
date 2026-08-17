#include "tc/parser/PrettyPrinter.h"
#include <vector>

namespace tc {

// helper
std::string formatIntList(const std::vector<int64_t>& values) {
    std::string result = "[";
    for (size_t i = 0; i < values.size(); i++) {
        if (i > 0) result += ", ";
        result += std::to_string(values[i]);
    }
    result += "]";
    return result;
}

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
            return "transpose(" + prettyPrint(*node.tensor) + ", " + formatIntList(node.perm) + ")";
        } else if constexpr (std::is_same_v<T, ReshapeNode>) {
            return "reshape(" + prettyPrint(*node.tensor) + ", " + formatIntList(node.new_shape) + ")";
        } else {
            static_assert(sizeof(T) == 0, "prettyPrint: unhandled node type");
        }
    }, expr.node);
}

}