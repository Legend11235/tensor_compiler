# Tensor Expression Compiler

A C++ tensor expression compiler and runtime with LLVM backend.

## Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
```

## Phases
| Phase | Description | Status |
|-------|-------------|--------|
| 1 | Tensor engine | 🔧 In progress |
| 2 | Parser + AST | ⏳ Planned |
| 3 | IR + passes | ⏳ Planned |
| 4 | LLVM backend | ⏳ Planned |
| 5 | Runtime | ⏳ Planned |
