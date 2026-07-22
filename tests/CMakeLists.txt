enable_testing()

add_executable(test_tensor test_tensor.cpp)
target_link_libraries(test_tensor PRIVATE tc_core)
add_test(NAME TensorTests COMMAND test_tensor)

add_executable(scratch_lexer scratch_lexer.cpp)
target_link_libraries(scratch_lexer PRIVATE tc_core)

add_executable(scratch_parser scratch_parser.cpp)
target_link_libraries(scratch_parser PRIVATE tc_core)

add_executable(test_parser test_parser.cpp)
target_link_libraries(test_parser PRIVATE tc_core)
add_test(NAME ParserTests COMMAND test_parser)