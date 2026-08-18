# Tensor Expression Compiler

A C++ compiler for a small tensor expression language, targeting a custom MLIR dialect.

Pipeline: lexer → recursive descent parser → AST → MLIRGen → custom `tc` MLIR dialect →
optimization passes → lowering to LLVM → native code.

## Status

| Phase | Description                                  | Status      |
|-------|-----------------------------------------------|-------------|
| 1     | Tensor engine (RAII, templates, matmul, elementwise ops, N-D transpose) | Done |
| 2     | Lexer, recursive descent parser, AST           | Done        |
| A     | MLIR environment (built from source)           | Done        |
| C     | Custom `tc` dialect (ODS/TableGen, 5 ops, `tc-opt` test tool) | Done |
| D     | MLIRGen: AST to dialect ops via `OpBuilder`    | Done        |
| E     | Fusion + tiling passes                         | Planned     |
| F     | Lowering (linalg → affine → scf → llvm)        | Planned     |
| G     | JIT execution, benchmarking                    | Planned     |

Current checkpoint: a tensor expression string parses to a typed AST, lowers through
MLIRGen into verified `tc` dialect MLIR, and prints. Confirmed for all five ops
(`matmul`, `add`, `relu`, `transpose`, `reshape`), individually and composed.

## Dialect

Five ops, each with ODS-declared type constraints, traits chosen from actual
mathematical properties, and a hand-written verifier wherever the correctness check
is relational rather than declarative (e.g. matmul's inner-dimension match,
transpose's perm-length-vs-rank, reshape's element-count match).

| Op | Operands | Attributes | Notes |
|---|---|---|---|
| `tc.matmul` | `lhs`, `rhs` (rank-2 `f32`) | — | Deliberately 2D-only, matching `linalg`'s split of `matmul`/`batch_matmul` |
| `tc.add` | `lhs`, `rhs` (any-rank `f32`) | — | Commutative, type-preserving |
| `tc.relu` | `input` (any-rank `f32`) | — | Type-preserving |
| `tc.transpose` | `input` (any-rank `f32`) | `perm` | N-D, not fixed 2D. Needed for attention-style axis permutation |
| `tc.reshape` | `input` (any-rank `f32`) | `new_shape` | Verifies element count is preserved |

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
./tools/tc-opt/tc-opt tests/mlir/matmul_valid.mlir
```