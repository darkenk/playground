/*
Open a file by creating an ifstream object called in. Make an ostrstream
object called os, and read the entire contents into the ostrstream using the
rdbuf( ) member function. Get the address of os’s char* with the str( )
function, and capitalize every character in the file using the Standard C
toupper( ) macro. Write the result out to a new file, and delete the memory
allocated by os.
*/
#include <gtest/gtest.h>
#include "fakefstream.hpp"

class Toupper : public ::testing::Test {
protected:
     virtual void TearDown() {
        FakeFstream::clear();
     }
};

template<class outstream = std::ofstream, class instream = std::ifstream>
bool toupper(const std::string& inFile, const std::string& outFile) {
    instream in(inFile);
    outstream out(outFile);
    if (in.fail() || out.fail()) {
        return false;
    }
    std::transform(std::istream_iterator<char>(in), std::istream_iterator<char>(),
                   std::ostream_iterator<char>(out), [](char c) { return std::toupper(c); });
    return true;
}

bool (*toupper_test)(const std::string&, const std::string&) = &toupper<FakeOFstream, FakeIFstream>;

//TEST_F(Toupper, does_it_compile_with_fstream) {
    template bool toupper(const std::string&, const std::string&);
//}

TEST_F(Toupper, create_empty_file_for_empty_file) {
    toupper_test("empty_file", "new_file");
    EXPECT_EQ(FakeOFstream::getFileContent("new_file"), "");
}

TEST_F(Toupper, copy_upper_case_file) {
    FakeIFstream::setFileContent("input_file", "AA");
    toupper_test("input_file", "out_file");
    EXPECT_EQ(FakeOFstream::getFileContent("out_file"), "AA");
}

TEST_F(Toupper, change_to_upper_case_file_aa) {
    FakeIFstream::setFileContent("input_file", "aa");
    toupper_test("input_file", "out_file");
    EXPECT_EQ(FakeOFstream::getFileContent("out_file"), "AA");
}

TEST_F(Toupper, change_to_upper_case_file_aA) {
    FakeIFstream::setFileContent("input_file", "aA");
    toupper_test("input_file", "out_file");
    EXPECT_EQ(FakeOFstream::getFileContent("out_file"), "AA");
}

TEST_F(Toupper, no_input_file_returns_false) {
    FakeFstream::setNoFile("input_file");
    bool ret = toupper_test("input_file", "out_file");
    EXPECT_FALSE(ret);
}

TEST_F(Toupper, no_output_file_returns_false) {
    FakeFstream::setNoFile("out_file");
    bool ret = toupper_test("input_file", "out_file");
    EXPECT_FALSE(ret);
}
