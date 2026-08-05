set(LLVM_TARGET_DEFINITIONS ../../include/tc/dialect/TensorDialect.td)
mlir_tablegen(TensorDialect.h.inc -gen-dialect-decls -dialect=tc)
mlir_tablegen(TensorDialect.cpp.inc -gen-dialect-defs -dialect=tc)
add_public_tablegen_target(TensorDialectIncGen)