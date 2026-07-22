# Tensor Expression Compiler

A C++ compiler for a small tensor expression language, targeting a custom MLIR dialect.

Pipeline: lexer → recursive descent parser → AST → MLIRGen → custom MLIR dialect →
optimization passes → lowering to LLVM → native code.

## Status

| Phase | Description                        | Status      |
|-------|-------------------------------------|-------------|
| 1     | Tensor engine (RAII, templates, matmul, elementwise ops) | Done |
| 2     | Lexer, parser, AST                  | In progress |
| 3     | MLIR dialect + MLIRGen              | Planned     |
| 4     | Optimization passes                 | Planned     |
| 5     | Lowering + codegen                  | Planned     |

## Build

```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
```
