cmake_minimum_required(VERSION 3.20)
project(TensorCompiler VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_compile_options(-Wall -Wextra -Wshadow -Wconversion -Wpedantic)

include_directories(include)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(benchmarks)
enable_testing()
