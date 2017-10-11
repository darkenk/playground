/* Modify WordCount.cpp so that it uses a multimap instead of a map. */

//: C07:WordCount.cpp
// From "Thinking in C++, 2nd Edition, Volume 2"
// by Bruce Eckel & Chuck Allison, (c) 2003 MindView, Inc.
// Available at www.BruceEckel.com.
// Count occurrences of words using a map
#include <gtest/gtest.h>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;

class Count {
    int i;
public:
    Count() : i(0) {}
    void operator++(int) { i++; } // Post-increment
    int& val() { return i; }
};

typedef multimap<string, Count> WordMap;
typedef WordMap::iterator WMIter;

void countAndDisplayWords(std::istream& input, std::ostream& output) {
    WordMap wordmap;
    string word;
    while(input >> word) {
        auto w = wordmap.find(word);
        if (w == wordmap.end()) {
            w = wordmap.emplace(word, Count());
        }
        w->second++;
    }
    for(WMIter w = wordmap.begin(); w != wordmap.end(); w++)
        output << (*w).first << ": "
             << (*w).second.val() << endl;
}

void main_func(int argc, char* argv[]) {
    std::string fname = "WordCount.cpp";
    if(argc > 1) fname = argv[1];
    ifstream in(fname);
    countAndDisplayWords(in, std::cout);
}

TEST(WordCount, main_loop_first_test) {
    auto in = std::istringstream("a a b b e");
    auto out = std::ostringstream();
    countAndDisplayWords(in, out);
    EXPECT_STREQ("a: 2\nb: 2\ne: 1\n", out.str().c_str());
}
