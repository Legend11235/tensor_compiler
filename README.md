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
| 1 | Tensor engine | ✅ Implemented |
| 2 | Parser + AST | 🔧 In progress |
| 3 | IR + passes | ⏳ Planned |
| 4 | LLVM backend | ⏳ Planned |
| 5 | Runtime | ⏳ Planned |
