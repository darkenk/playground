include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Add test cpp file
add_executable(test1 testgtest.cpp)
add_executable(test2 testgtest2.cpp)
# Link test executable against gtest & gtest_main
target_link_libraries(test1 gtest gtest_main)
target_link_libraries(test2 gtest gtest_main)
