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

template<class ostream = std::ofstream, class istream = std::ifstream>
void copyright(const std::string& fileName, const std::string& copyrightPattern) {
    istream in(fileName);
    ostream out(fileName);
    out <<  copyrightPattern << std::endl << in.rdbuf();
}

void (*copyright_test)(const std::string&, const std::string&) = &copyright<FakeOFstream, FakeIFstream>;

TEST_F(Copyright, append_text_to_file) {
    FakeIFstream::setFileContent(input_file, "aa");
    copyright_test(input_file, "bb");
    EXPECT_EQ("bb\naa", FakeOFstream::getFileContent(input_file));
}
