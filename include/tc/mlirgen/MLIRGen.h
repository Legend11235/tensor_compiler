#pragma once
#include "mlir/IR/BuiltinOps.h"
#include "tc/parser/AST.h"

namespace tc {
mlir::ModuleOp mlirGen(mlir::MLIRContext &context, const Expr &expr);
}