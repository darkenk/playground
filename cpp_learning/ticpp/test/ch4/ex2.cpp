/* Create a kind of “hangman” game. Create a class that contains a char and a
bool to indicate whether that char has been guessed yet. Randomly select a
word from a file, and read it into a vector of your new type. Repeatedly ask
the user for a character guess, and after each guess display the characters in
the word that have been guessed, and underscores for the characters that
haven’t. Allow a way for the user to guess the whole word. Decrement a
value for each guess, and if the user can get the whole word before the value
goes to zero, they win.
*/

#include <gtest/gtest.h>
#include <fstream>

// Helpers
class Letter {
public:
    Letter(const char letter): mLetter{letter} {
        mGuessed = false;
    }

    char get() const {
        return mGuessed ? mLetter : '_';
    }

    bool guess(const char letter) {
        return mGuessed |= letter == mLetter;
    }

    bool guessed() const {
        return mGuessed;
    }

    char show() const {
        return mLetter;
    }

private:
    const char mLetter;
    bool mGuessed;
};

class Word {
public:
    Word(const std::string word) {
        std::copy(word.begin(), word.end(), std::back_inserter(mWord));
    }

    std::string get() {
        std::string word;
        std::transform(mWord.begin(), mWord.end(), std::back_inserter(word),
                       [](const Letter& letter) {return letter.get(); });
        return word;
    }

    bool guess(const char letter) {
        bool ret = false;
        for (auto& l : mWord) {
            ret |= l.guess(letter);
        }
        return ret;
    }

    bool allLetterGuessed() {
        for (const auto& l : mWord) {
            if (not l.guessed()) {
                return false;
            }
        }
        return true;
    }

    std::string show() {
        std::string word;
        std::transform(mWord.begin(), mWord.end(), std::back_inserter(word),
                       [](const Letter& letter) {return letter.show(); });
        return word;
    }

private:
    std::vector<Letter> mWord;
};

std::string readRandomWord(std::istream& in) {
    std::vector<std::string> words;
    std::string s;
    while (in >> s) {
        words.push_back(s);
    }
    // don't know how to test this part
    return words[rand() % words.size()];
}

void hangman(Word& word, std::istream& in = std::cin, std::ostream& out = std::cout) {
    char letter;
    int tries = 5;
    do {
        in >> letter;
        if (!word.guess(letter)) {
            tries--;
        }
        out << word.get();
    } while (tries > 0 && !word.allLetterGuessed());
}

void displayResult(Word& word, std::ostream& out = std::cout) {
    if (word.allLetterGuessed()) {
        out << "You guessed the word: " << word.show();
    } else {
        out << "You failed to guess the word: " << word.show();
    }
}

// Game
void mainLoop() {
    srand(static_cast<unsigned int>(time(NULL)));
    auto fileStream = std::ifstream("sample.txt");
    auto word = Word(readRandomWord(fileStream));
    hangman(word);
    displayResult(word);
}

// Tests
TEST(WordTest, five_letter_word_returns_five_underscores) {
    Word w("abcdf");
    EXPECT_STREQ("_____", w.get().c_str());
}

TEST(WordTest, guessed_letter_returns_true) {
    Word w("abcdf");
    EXPECT_TRUE(w.guess('a'));
}

TEST(WordTest, not_guessed_letter_returns_false) {
    Word w("abcdf");
    EXPECT_FALSE(w.guess('z'));
}

TEST(WordTest, guessed_letter_is_shown) {
    Word w("abcdf");
    w.guess('a');
    EXPECT_STREQ("a____", w.get().c_str());
}

TEST(WordTest, return_all_guessed_letter_true_if_all_letters_were_guessed) {
    Word w("abcdf");
    w.guess('a');
    w.guess('b');
    w.guess('c');
    w.guess('d');
    w.guess('f');
    EXPECT_TRUE(w.allLetterGuessed());
}

TEST(WordTest, show_word) {
    Word w("abcdf");
    EXPECT_STREQ("abcdf", w.show().c_str());
}

TEST(ReadWord, return_word_if_there_is_only_one_word) {
    auto word = "aaa";
    auto in = std::istringstream(word);
    EXPECT_EQ(word, readRandomWord(in));
}

TEST(HangmanTest, after_5_wrong_tries_game_ends) {
    Word w("abcdf");
    auto in = std::istringstream("g g g g g");
    auto out = std::ostringstream();
    hangman(w, in, out);
    EXPECT_EQ(false, w.allLetterGuessed());
}

TEST(HangmanTest, after_5_correct_guesses_game_ends) {
    Word w("abcdf");
    auto in = std::istringstream("a b c d f");
    auto out = std::ostringstream();
    hangman(w, in, out);
    EXPECT_EQ(true, w.allLetterGuessed());
}

TEST(HangmanTest, underscores_are_displayed_if_first_guess_is_incorrect) {
    Word w("abcdf");
    auto in = std::istringstream("g a b c d f");
    auto out = std::ostringstream();
    hangman(w, in, out);
    EXPECT_STREQ("_____", out.str().substr(0, 5).c_str());
}

TEST(HangmanTest, second_letter_is_displayed_if_second_letter_is_guessed) {
    Word w("abcdf");
    auto in = std::istringstream("b a c d f");
    auto out = std::ostringstream();
    hangman(w, in, out);
    EXPECT_STREQ("_b___", out.str().substr(0, 5).c_str());
}
