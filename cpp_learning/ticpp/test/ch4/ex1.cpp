/* Create a set<char>, then open a file (whose name is provided on the
command line) and read that file in a char at a time, placing each char in
the set. Print the results and observe the organization, and whether there are
any letters in the alphabet that are not used in that particular file.
*/
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "fakefstream.hpp"

class SetTest : public ::testing::Test {
public:
    const std::string input_file = "input_file";
    std::stringstream out;
    const std::string content = "aaaabbbddda fs";
protected:
    virtual void SetUp() {
        FakeFstream::setFileContent(input_file, content);
    }

    virtual void TearDown() {
        FakeFstream::clear();
    }
};

template<class istream = std::ifstream>
std::set<char> readCharFromFileIntoSet(const std::string& file) {
    istream instream(file);
    return {std::istream_iterator<char>(instream), std::istream_iterator<char>()};
}

TEST_F(SetTest, readCharFromFileIntoSet) {
    std::set<char> chars = readCharFromFileIntoSet<FakeIFstream>(input_file);
    EXPECT_THAT(chars, testing::ElementsAre('a', 'b', 'd', 'f', 's'));
}
