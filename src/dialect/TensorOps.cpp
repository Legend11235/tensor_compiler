#include "tc/dialect/TensorOps.h"

#define GET_OP_CLASSES
#include "TensorOps.cpp.inc"

::llvm::LogicalResult mlir::tc::MatMulOp::verify() {

    auto lhsType = ::llvm::cast<mlir::RankedTensorType>(getLhs().getType());
    auto rhsType = ::llvm::cast<mlir::RankedTensorType>(getRhs().getType());

    auto lhsShape = lhsType.getShape();
    auto rhsShape = rhsType.getShape();

    // check if its (M,N) x (N,M) for valid multiplication
    if (lhsShape[1] != rhsShape[0]) {
        return emitOpError("inner dimension must match: lhs has ") << lhsShape[1] << ", rhs has " << rhsShape[0];
    }

  return ::llvm::success();
}