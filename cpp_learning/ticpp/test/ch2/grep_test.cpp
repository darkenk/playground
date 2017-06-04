/*
Create a program that opens a file (the first argument on the command line)
and searches it for any one of a set of words (the remaining arguments on
the command line). Read the input a line at a time, and print out the lines
(with line numbers) that match.
*/
#include <gtest/gtest.h>
#include "fakefstream.hpp"

class Grep : public ::testing::Test {
public:
    const std::string input_file = "input_file";
    std::stringstream out;
protected:
     virtual void TearDown() {
        FakeFstream::clear();
     }
};

template<class instream = std::ifstream>
void grep(std::ostream& out, const std::string& fileName, std::initializer_list<const std::string> pattern) {
    instream in(fileName);
    if (in.fail()) {
        out << "No such file: " << fileName;
        return;
    }
    int i = 0;
    std::string line;
    while(std::getline(in, line)) {
        i++;
        for (const std::string& s : pattern) {
            if (line.find(s) != std::string::npos) {
                out << i << ": " << line;
                break;
            }
        }
    }
}

void (*grep_test)(std::ostream&, const std::string&, std::initializer_list<const std::string>) = &grep<FakeIFstream>;

TEST_F(Grep, print_no_such_file_if_there_is_no_input_file) {
    FakeIFstream::setNoFile(input_file);
    grep_test(out, input_file, {"aa"});
    EXPECT_EQ("No such file: input_file", out.str());
}

TEST_F(Grep, print_matching_line) {
    FakeIFstream::setFileContent(input_file, "aa");
    grep_test(out, input_file, {"aa"});
    EXPECT_EQ("1: aa", out.str());
}

TEST_F(Grep, print_nothing_if_there_is_no_match) {
    FakeIFstream::setFileContent(input_file, "bb");
    grep_test(out, input_file, {"aa"});
    EXPECT_EQ("", out.str());
}

TEST_F(Grep, print_matching_line_when_looking_for_two_words) {
    FakeIFstream::setFileContent(input_file, "bb");
    grep_test(out, input_file, {"aa", "bb"});
    EXPECT_EQ("1: bb", out.str());
}

TEST_F(Grep, print_second_matching_line_when_looking_for_two_words) {
    FakeIFstream::setFileContent(input_file, "cc\nbb dd");
    grep_test(out, input_file, {"aa", "bb"});
    EXPECT_EQ("2: bb dd", out.str());
}
