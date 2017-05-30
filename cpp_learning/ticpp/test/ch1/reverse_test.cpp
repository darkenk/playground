#include "gtest/gtest.h"

void reverse(std::string& str) {
    std::reverse(str.begin(), str.end());
}

std::string reverse(const std::string& str) {
    std::string ret;
    std::copy(str.rbegin(), str.rend(), std::back_inserter(ret));
    return ret;
}

TEST(Reverse, reverse_one_letter) {
    std::string arg = "a";
    reverse(arg);
    EXPECT_EQ(arg, "a");
}

TEST(Revese, reverse_two_letter) {
    std::string arg = "ab";
    reverse(arg);
    EXPECT_EQ(arg, "ba");
}

TEST(Revese, reverse_three__letter) {
    std::string arg = "abc";
    reverse(arg);
    EXPECT_EQ(arg, "cba");
}

TEST(Reverse, reverse_const_one_letter) {
    EXPECT_EQ(reverse("a"), "a");
}

TEST(Revese, reverse_const_two_letter) {
    EXPECT_EQ(reverse("ab"), "ba");
}

TEST(Revese, reverse_const_three__letter) {
    EXPECT_EQ(reverse("abc"), "cba");
}
