#include "tc/dialect/TensorOps.h"

#define GET_OP_CLASSES
#include "TensorOps.cpp.inc"

// custom verify for matmul
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


// custom verify for transpose
::llvm::LogicalResult mlir::tc::TransposeOp::verify() {
  auto inputType = ::llvm::cast<mlir::RankedTensorType>(getInput().getType());
  auto perm = getPerm();

  if (perm.size() != static_cast<size_t>(inputType.getRank())) {
    return emitOpError("perm size must match input rank: input has rank ")
        << inputType.getRank() << ", perm has size " << perm.size();
  }

  return ::llvm::success();
}


//custom verify for reshape
::llvm::LogicalResult mlir::tc::ReshapeOp::verify() {
   auto inputType = ::llvm::cast<mlir::RankedTensorType>(getInput().getType());
   auto newShape = getNewShape();

   int64_t inputTotalElems = 1;
   for (int64_t dim : inputType.getShape()) {
      inputTotalElems *= dim;
   }

   int64_t newShapeTotalElems = 1;
   for (int64_t dim : newShape) {
      newShapeTotalElems *= dim;
   }

   if(inputTotalElems != newShapeTotalElems) {
     return emitOpError("new_shape must have same total element count as input: input has ") << inputTotalElems << ", new_shape has " << newShapeTotalElems;
   }

   return ::llvm::success();
}