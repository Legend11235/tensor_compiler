add_llvm_executable(tc-opt tc-opt.cpp)

llvm_update_compile_flags(tc-opt)

target_link_libraries(tc-opt PRIVATE
  tc_dialect
  -Wl,-force_load,$<TARGET_FILE:MLIRIR>
  MLIRFuncDialect
  MLIROptLib
)

