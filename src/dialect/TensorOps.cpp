#include "tc/dialect/TensorOps.h"

#define GET_OP_CLASSES
#include "TensorOps.cpp.inc"

// custom verify for matmul
::llvm::LogicalResult mlir::tc::MatMulOp::verify() {

    auto lhsType = ::llvm::cast<mlir::RankedTensorType>(getLhs().getType());
    auto rhsType = ::llvm::cast<mlir::RankedTensorType>(getRhs().getType());
    auto result = ::llvm::cast<mlir::RankedTensorType>(getResult().getType());

    auto lhsShape = lhsType.getShape();
    auto rhsShape = rhsType.getShape();
    auto resultShape = result.getShape();

    // only report a mismatch if both inner dims are static 
    if((!mlir::ShapedType::isDynamic(lhsShape[1])) && (!mlir::ShapedType::isDynamic(rhsShape[0]))){
      // check if its (M,K) x (K,N) -> (M, N) for valid multiplication
      if (lhsShape[1] != rhsShape[0]) {
          return emitOpError("inner dimension must match: lhs has ") << lhsShape[1] << ", rhs has " << rhsShape[0];
      }
   }


    // only report a mismatch if both outer dims are static 
    // this actually didn't catch the case were the result itself had a dynamic shape
    /*
    if((!mlir::ShapedType::isDynamic(lhsShape[0])) && (!mlir::ShapedType::isDynamic(rhsShape[1]))){
      // Will also have to check the result shape
      if(resultShape[0]!= lhsShape[0] || resultShape[1] != rhsShape[1]){
          return emitOpError("result dimensions do not match:") << "should be (" << lhsShape[0] << "," << rhsShape[1]<<")";
      }
    }
    */

    // rewrite that commeneted block above with two seperate if statements to account for dynamic shape for result
    // gaurds to check of dimensions of the result is static and not dynamic
    if((!mlir::ShapedType::isDynamic(resultShape[0])) && (!mlir::ShapedType::isDynamic(lhsShape[0]))){
      if(resultShape[0] != lhsShape[0]){
        return emitOpError("result dimensions do not match: ") << "should be (" << lhsShape[0] << "," << rhsShape[1]<<")";
      }
    }

    if((!mlir::ShapedType::isDynamic(resultShape[1])) && (!mlir::ShapedType::isDynamic(rhsShape[1]))){
      if(resultShape[1] != rhsShape[1]){
        return emitOpError("result dimensions do not match: ") << "should be (" << lhsShape[0] << "," << rhsShape[1]<<")";
      }
    }
   

  return ::llvm::success();
}


// custom verify for transpose
::llvm::LogicalResult mlir::tc::TransposeOp::verify() {
  auto inputType = ::llvm::dyn_cast<mlir::RankedTensorType>(getInput().getType());
  if (!inputType) {
    // unranked input: rank isn't known, so the perm-size-vs-rank check can't run
    return ::llvm::success();
  }
  auto perm = getPerm();
  if (perm.size() != static_cast<size_t>(inputType.getRank())) {
    return emitOpError("perm size must match input rank: input has rank ")
        << inputType.getRank() << ", perm has size " << perm.size();
  }
  return ::llvm::success();
}


//custom verify for reshape
::llvm::LogicalResult mlir::tc::ReshapeOp::verify() {
   auto inputType = ::llvm::dyn_cast<mlir::RankedTensorType>(getInput().getType());
   if (!inputType) {
     return ::llvm::success();
   }
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