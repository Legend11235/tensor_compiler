#include "tc/dialect/TensorDialect.h"
#include "tc/dialect/TensorOps.h"

#include "TensorDialect.cpp.inc"

void mlir::tc::TensorDialect::initialize() {
    addOperations <
        #define GET_OP_LIST
        #include "TensorOps.cpp.inc"
    >();
}