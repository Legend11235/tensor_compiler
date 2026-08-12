#ifndef TC_DIALECT_TENSOROPS_H
#define TC_DIALECT_TENSOROPS_H

#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/IR/OpImplementation.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"
#include "tc/dialect/TensorDialect.h"

#define GET_OP_CLASSES
#include "TensorOps.h.inc"

#endif // TC_DIALECT_TENSOROPS_H