#pragma once
#include "tc/parser/AST.h"
#include <string>

namespace tc {
    std::string prettyPrint(const Expr& expr);
}