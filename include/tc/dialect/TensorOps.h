#ifndef TC_DIALECT_TENSOROPS_H
#define TC_DIALECT_TENSOROPS_H

#include "mlir/IR/OpDefinition.h"
#include "tc/dialect/TensorDialect.h"

#define GET_OP_CLASSES
#include "TensorOps.h.inc"

#endif // TC_DIALECT_TENSOROPS_H