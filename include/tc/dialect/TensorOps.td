#ifndef TC_OPS
#define TC_OPS

include "mlir/IR/OpBase.td"
include "tc/dialect/TensorDialect.td"

class TC_Op<string mnemonic, list<Trait> traits = []> :
    Op<TensorDialect, mnemonic, traits>;

#endif // TC_OPS