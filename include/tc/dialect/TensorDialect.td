#ifndef TC_DIALECT
#define TC_DIALECT

include "mlir/IR/OpBase.td"

def TensorDialect : Dialect {
  let name = "tc";
  let summary = "A Dialect for Tensor expression compilation";
  let description = [{
    The `tc` dialect models tensor operations (matmul, elementwise ops,
    reshape, transpose) as a target for the tensor expression compiler
    frontend, prior to lowering into standard MLIR dialects.
  }];
  let cppNamespace = "::mlir::tc";
}

#endif // TC_DIALECT