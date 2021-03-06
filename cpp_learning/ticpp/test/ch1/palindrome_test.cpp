#include "gtest/gtest.h"

bool isPalindrome(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    for (std::string::size_type i = 0; i < str.size()/2; i++) {
        if (str.at(i) != str.at(str.size()-(i+1))) {
            return false;
        }
    }
    return true;
}

TEST(Palindrome, empty_string_is_not_palindrome) {
    EXPECT_FALSE(isPalindrome(""));
}

TEST(Palindrome, one_letter_string_is_palindrome) {
    EXPECT_TRUE(isPalindrome("a"));
}

TEST(Palindrome, two_same_letter_is_palindrome) {
    EXPECT_TRUE(isPalindrome("aa"));
}

TEST(Palindrome, two_not_the_same_letter_is_palindrome) {
    EXPECT_FALSE(isPalindrome("ab"));
}

TEST(Palindrome, three_same_letter_is_palindrome) {
    EXPECT_TRUE(isPalindrome("aaa"));
}

TEST(Palindrome, three_letter_palindrome_is_palindrome) {
    EXPECT_TRUE(isPalindrome("aba"));
}

TEST(Palindrome, three_letter_not_palindrome_is_not_palindrome) {
    EXPECT_FALSE(isPalindrome("abb"));
}
