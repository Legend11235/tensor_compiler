#ifndef TC_OPS
#define TC_OPS

include "mlir/IR/OpBase.td"
include "tc/dialect/TensorDialect.td"
include "mlir/IR/CommonTypeConstraints.td"
include "mlir/Interfaces/SideEffectInterfaces.td"
include "mlir/Interfaces/InferTypeOpInterface.td"

class TC_Op<string mnemonic, list<Trait> traits = []> :
    Op<TensorDialect, mnemonic, traits>;

    // dialect for matmul
    // Pure just means that input will always prodice the same output. self note: this is helpful for stuff like deadcode elmination
    def MatMulOp : TC_Op<"matmul", [Pure]> {
        let summary = "matrix multiplication operation";
        let description = [{ Performs matrix multiplication between two rank-2 tensors. Given lhs of shape (M, K) and rhs of shape (K, N), produces a result of shape (M, N) }];

        // specify shape and type of arguments and result
        let arguments = (ins 2DTensorOf<[F32]>:$lhs, 2DTensorOf<[F32]>:$rhs);
        let results = (outs 2DTensorOf<[F32]>:$result);

        // matmul will need a custom verifier because we have to check if inner dimensions match
        let hasVerifier = 1;
        // custom tetx syntax for my op
        let assemblyFormat = "$lhs `,` $rhs attr-dict `:` type($lhs) `,` type($rhs) `->` type($result)";
    }

    // matrix addition dialect
    // Commutative is a trait, because matrix addition is cummutative
    // SameOperandsAndResultType because the two tensors being added (operands) and the resulting tensor(result) will have the same dimension and type 
    def AddOp : TC_Op<"add", [Pure, Commutative, SameOperandsAndResultType]> {
        let summary = "elementwise addition operation";
        let description =[{
            Performs elementwise addition between two tensors of identical shape
        }];

        let arguments = (ins TensorOf<[F32]>:$lhs, TensorOf<[F32]>:$rhs);
        let results = (outs TensorOf<[F32]>:$result);
        
        // here we dont set hasVerifier = 1 because of elementwise addition the trait "SameOperandsAndResultType" already takes care of it
        
        let assemblyFormat = "$lhs `,` $rhs attr-dict `:` type($result)";
    }

    // Relu operation
    def ReluOp : TC_Op<"relu", [Pure, SameOperandsAndResultType]> {
        let summary = "element wise relu operation";
        let description = [{ 
            Performs element wise relu operation
        }];

        let arguments = (ins TensorOf<[F32]>:$input);
        let results = (outs TensorOf<[F32]>:$result);

        let assemblyFormat = "$input attr-dict `:` type($result)";
    }

#endif // TC_OPS