project(cpp_tests)
cmake_minimum_required(VERSION 2.8)

find_package(dk_utils REQUIRED)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)
add_subdirectory(tests)
