cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)

find_package(dk_utils REQUIRED)

include(dk_compiler_flags)
add_subdirectory(src)
add_subdirectory(test)
