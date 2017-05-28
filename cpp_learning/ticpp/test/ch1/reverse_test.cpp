#include "gtest/gtest.h"

std::string& reverse(std::string& str) {
    std::reverse(str.begin(), str.end());
    return str;
}

TEST(Reverse, reverse_one_letter) {
    std::string arg = "a";
    EXPECT_EQ(reverse(arg), arg);
}

TEST(Revese, reverse_two_letter) {
    std::string arg = "ab";
    std::string ret = "ba";
    EXPECT_EQ(reverse(arg), ret);
}

TEST(Revese, reverse_three__letter) {
    std::string arg = "abc";
    std::string ret = "cba";
    EXPECT_EQ(reverse(arg), ret);
}
