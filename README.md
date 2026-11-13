# Tensor Expression Compiler

A C++ compiler for a small tensor expression language, built on MLIR with a custom `tc` dialect.

Working today: lexer → recursive descent parser → AST → MLIRGen → verified `tc` dialect MLIR.
Planned: optimization passes → lowering through `linalg` to LLVM → native code.

## Example

A program is a single expression over named input tensors:

```
add(relu(x1), transpose(x2, [1, 0]))
```

MLIRGen turns each input into a function argument and each call into a `tc` op:

```mlir
module {
  func.func @main(%arg0: tensor<*xf32>, %arg1: tensor<*xf32>) -> tensor<*xf32> {
    %0 = tc.relu %arg0 : tensor<*xf32>
    %1 = tc.transpose %arg1, [1, 0] : tensor<*xf32> -> tensor<*xf32>
    %2 = tc.add %0, %1 : tensor<*xf32>
    return %2 : tensor<*xf32>
  }
}
```

(Output of `./tests/test_mlirgen`.)

## Status

| Phase | Description | Status |
|-------|-------------|--------|
| 1 | Tensor engine (RAII, templates, matmul, elementwise ops, N-D transpose) | Done |
| 2 | Lexer, recursive descent parser, AST | Done |
| A | MLIR environment (built from source) | Done |
| C | Custom `tc` dialect (ODS/TableGen, 5 ops, `tc-opt` test tool) | Done |
| D | MLIRGen: AST to dialect ops via `OpBuilder` | Done |
| E | Fusion + tiling passes | Planned |
| F | Lowering (linalg → affine → scf → llvm) | Planned |
| G | JIT execution, benchmarking | Planned |

## Dialect

Five ops. Type constraints are declared in ODS, traits match each op's actual properties,
and hand-written verifiers cover the checks ODS can't express:

- `matmul`: inner dimensions match, result shape is M x N
- `transpose`: `perm` is a valid permutation of the input rank (right length, in range, no duplicates)
- `reshape`: no negative entries in `new_shape`, element count is preserved

Shape checks are skipped when a dimension is dynamic (`?`), since it could still be correct at runtime.

| Op | Operands | Attributes | Notes |
|---|---|---|---|
| `tc.matmul` | `lhs`, `rhs` (rank-2 `f32`) | none | 2D only, like `linalg`'s split of `matmul` / `batch_matmul` |
| `tc.add` | `lhs`, `rhs` (any-rank `f32`) | none | Commutative, type-preserving |
| `tc.relu` | `input` (any-rank `f32`) | none | Type-preserving |
| `tc.transpose` | `input` (any-rank `f32`) | `perm` | N-D, for attention-style axis permutations |
| `tc.reshape` | `input` (any-rank `f32`) | `new_shape` | Preserves element count |

## Known limitations

- The language has no shape annotations yet, so MLIRGen emits unranked tensors and most
  shape checks only fire on hand-written static-shape MLIR (which is what the tests use).
- `tc.transpose` doesn't check the declared result shape against the permuted input yet.
- MLIRGen emits `tensor.cast` for matmul operands, but `tc-opt` doesn't register the
  `tensor` dialect yet.

## Next

Shape annotations in the language plus `InferTypeOpInterface` on the ops, then a
`tc` → `linalg` conversion pass so programs can be lowered and run.

## Build

```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
```

Requires LLVM/MLIR built from source (see `CMakeLists.txt` for the expected
`MLIRConfig.cmake` path).

Run tests:

```
ctest --output-on-failure
./tools/tc-opt/tc-opt ../tests/mlir/matmul_valid.mlir
./tests/test_mlirgen
```