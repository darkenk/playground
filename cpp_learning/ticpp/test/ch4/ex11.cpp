/*Open a text file whose name is provided on the command line. Read the file
a word at a time (hint: use >>) and use a multiset<string> to create a word
count for each word.
*/

#include <gtest/gtest.h>
#include <iostream>
#include <set>

void countWords(std::istream& file, std::ostream& out = std::cout) {
    std::multiset<std::string> words;
    std::string word;
    while (file >> word) {
        words.insert(word);
    }
    if (words.size() == 0) {
        out << "no words";
    }
    std::set<std::string> uniqueWords(words.begin(), words.end());
    for (auto w : uniqueWords) {
        out << w << ": " << words.count(w) << std::endl;
    }
}

TEST(CountWords, empty_set_returns_no_words) {
    auto in = std::istringstream();
    auto out = std::ostringstream();
    countWords(in, out);
    EXPECT_EQ("no words", out.str());
}

TEST(CountWords, one_word_file_returns_word_and_count) {
    auto in = std::istringstream("aa");
    auto out = std::ostringstream();
    countWords(in, out);
    EXPECT_EQ("aa: 1\n", out.str());
}

TEST(CountWords, same_words_are_counted_correctly) {
    auto in = std::istringstream("aa aa aa");
    auto out = std::ostringstream();
    countWords(in, out);
    EXPECT_EQ("aa: 3\n", out.str());
}

TEST(CountWords, multiple_words_are_counted_correctly) {
    auto in = std::istringstream("aa aa bb");
    auto out = std::ostringstream();
    countWords(in, out);
    EXPECT_EQ("aa: 2\nbb: 1\n", out.str());
}
