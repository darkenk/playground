/*
Create a generator that produces random int values between 0 and 20. Use
this to fill a multiset<int>. Count the occurrences of each value, following
the example given in MultiSetWordCount.cpp.
*/

#include <gtest/gtest.h>

#include <string>
#include <set>
#include <fstream>
#include <iterator>

void countAndDisplayNumbers(const std::multiset<int>& data, std::ostream& output) {
    auto end = data.cend();
    auto it = data.cbegin();
    while (it != end) {
        auto p = data.equal_range(*it);
        output << *it << ": " << std::distance(p.first, p.second) << "\n";
        it = p.second;
    }
}

// How to test random?
void main_func() {
    srand(static_cast<unsigned int>(std::time(nullptr)));
    std::multiset<int> data;
    for (int i = 0; i < 50; i++) {
        data.insert(rand() % 21);
    }
    countAndDisplayNumbers(data, std::cout);
}

TEST(RandomGenerator, display_one_number) {
    std::stringstream out;
    std::multiset<int> data{0};
    countAndDisplayNumbers(data, out);
    EXPECT_STREQ("0: 1\n", out.str().c_str());
}

TEST(RandomGenerator, display_two_numbers) {
    std::stringstream out;
    std::multiset<int> data{0, 1, 0};
    countAndDisplayNumbers(data, out);
    EXPECT_STREQ("0: 2\n1: 1\n", out.str().c_str());
}
