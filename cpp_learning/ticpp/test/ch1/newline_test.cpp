#include "gtest/gtest.h"

std::string& replaceNewLine(std::string& str) {
    static const std::string badNewline = "\r\n";
    static const std::string goodNewLine = "\n";
    for (auto pos = str.find(badNewline); pos != std::string::npos; pos = str.find(badNewline)) {
        str.replace(pos, badNewline.size(), goodNewLine);
    }
    return str;
}

TEST(NewLine, empty_string_return_empty_string) {
    std::string arg = "";
    EXPECT_EQ(replaceNewLine(arg), arg);
}

TEST(NewLine, replace_one_double_character_newline) {
    std::string arg = "\r\n";
    EXPECT_EQ(replaceNewLine(arg), "\n");
}

TEST(NewLine, no_replacement_if_no_bad_newline) {
    std::string arg = "lorem impsum";
    EXPECT_EQ(replaceNewLine(arg), "lorem impsum");
}

TEST(NewLine, replace_two_bad_newlines) {
    std::string arg = "ffa\r\nffa\r\n";
    EXPECT_EQ(replaceNewLine(arg), "ffa\nffa\n");
}

TEST(NewLine, replace_many_bad_lines) {
    std::string arg = "\r\n\r\n\r\n\r\n\r\n";
    EXPECT_EQ(replaceNewLine(arg), "\n\n\n\n\n");
}
