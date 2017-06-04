/*
Write a program that adds a copyright notice to the beginning of all source-
code files. This is a small modification to exercise 1.
*/
#include <gtest/gtest.h>
#include "fakefstream.hpp"

class Copyright : public ::testing::Test {
public:
    const std::string input_file = "input_file";
    std::stringstream out;
protected:
     virtual void TearDown() {
        FakeFstream::clear();
     }
};

template<class ostream = std::ofstream>
void copyright(const std::string& fileName, const std::string& copyrightPattern) {
    ostream out(fileName, std::ios_base::out);
    out << copyrightPattern;
//    << in.rdbuf();
}

void (*copyright_test)(const std::string&, const std::string&) = &copyright<FakeOFstream>;

TEST_F(Copyright, append_text_to_file) {
    FakeIFstream::setFileContent(input_file, "aa");
    copyright_test(input_file, "bb");
    EXPECT_EQ("bb\naa", FakeIFstream::getFileContent(input_file));
}
