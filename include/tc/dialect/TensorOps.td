#ifndef TC_OPS
#define TC_OPS

include "mlir/IR/OpBase.td"
include "tc/dialect/TensorDialect.td"
include "mlir/IR/CommonTypeConstraints.td"
include "mlir/Interfaces/SideEffectInterfaces.td"

class TC_Op<string mnemonic, list<Trait> traits = []> :
    Op<TensorDialect, mnemonic, traits>;

    def MatMulOp : TC_Op<"matmul", [Pure]> {
        let summary = "matrix multiplication operation";
        let description = [{ Performs matrix multiplication between two rank-2 tensors. Given lhs of shape (M, K) and rhs of shape (K, N), produces a result of shape (M, N) }];

        let arguments = (ins 2DTensorOf<[F32]>:$lhs, 2DTensorOf<[F32]>:$rhs);
        let results = (outs 2DTensorOf<[F32]>:$result);

        let hasVerifier = 1;
        let assemblyFormat = "$lhs `,` $rhs attr-dict `:` type($lhs) `,` type($rhs) `->` type($result)";
}

#endif // TC_OPS