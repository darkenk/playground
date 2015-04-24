set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin_tests)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Add test cpp file
add_executable(ParserTest parser_test.cpp ../parser.cpp)
# Link test executable against gtest & gtest_main
target_link_libraries(ParserTest gtest gtest_main)
