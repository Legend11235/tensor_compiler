add_library(tc_mlirgen
  MLIRGen.cpp
)
target_include_directories(tc_mlirgen PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tc_mlirgen PUBLIC tc_dialect tc_core MLIRIR MLIRFuncDialect MLIRTensorDialect)
