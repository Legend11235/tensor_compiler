enable_testing()

add_executable(test_tensor test_tensor.cpp)
target_link_libraries(test_tensor PRIVATE tc_core)
add_test(NAME TensorTests COMMAND test_tensor)
