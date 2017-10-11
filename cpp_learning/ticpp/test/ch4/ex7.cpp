/*Modify Reversible.cpp so it works with deque and list instead of vector.*/

#include <gtest/gtest.h>

//: C07:Reversible.cpp
// From "Thinking in C++, 2nd Edition, Volume 2"
// by Bruce Eckel & Chuck Allison, (c) 2001 MindView, Inc.
// Available at www.BruceEckel.com.
// Using reversible containers
//{L} ../TestSuite/Test
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <list>
using namespace std;

template<class T>
void reverseStringAndDisplay(std::istream& input, std::ostream& output) {
    string line;
    T lines;
    while(getline(input, line))
        lines.push_back(line);
    typename T::reverse_iterator r;
    for(r = lines.rbegin(); r != lines.rend(); r++)
        output << *r << endl;
}

void main_func() {
    ifstream in("Reversible.cpp");
    reverseStringAndDisplay<vector<string>>(in, cout);
}

TEST(Reversible, test_vector) {
    std::stringstream input{"aa a\nbb b\n"};
    std::stringstream output{};
    reverseStringAndDisplay<std::vector<std::string>>(input, output);
    EXPECT_STREQ("bb b\naa a\n", output.str().c_str());
}

TEST(Reversible, test_list) {
    std::stringstream input{"aa a\nbb b\n"};
    std::stringstream output{};
    reverseStringAndDisplay<std::list<std::string>>(input, output);
    EXPECT_STREQ("bb b\naa a\n", output.str().c_str());
}

TEST(Reversible, test_deque) {
    std::stringstream input{"aa a\nbb b\n"};
    std::stringstream output{};
    reverseStringAndDisplay<std::deque<std::string>>(input, output);
    EXPECT_STREQ("bb b\naa a\n", output.str().c_str());
}

