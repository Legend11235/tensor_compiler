add_library(tc_core
  tensor/Tensor.cpp
  parser/Lexer.cpp
  parser/Parser.cpp
  parser/PrettyPrinter.cpp
)
target_include_directories(tc_core PUBLIC ${CMAKE_SOURCE_DIR}/include)
add_subdirectory(dialect)