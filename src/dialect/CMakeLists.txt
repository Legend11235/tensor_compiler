set(LLVM_TARGET_DEFINITIONS ../../include/tc/dialect/TensorDialect.td)
mlir_tablegen(TensorDialect.h.inc -gen-dialect-decls -dialect=tc)
mlir_tablegen(TensorDialect.cpp.inc -gen-dialect-defs -dialect=tc)
add_public_tablegen_target(TensorDialectIncGen)

add_library(tc_dialect
  TensorDialect.cpp
)
add_dependencies(tc_dialect TensorDialectIncGen)
target_include_directories(tc_dialect PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tc_dialect PUBLIC MLIRIR)
target_include_directories(tc_dialect PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_compile_options(tc_dialect PRIVATE -Wno-shadow -Wno-conversion -Wno-sign-conversion)