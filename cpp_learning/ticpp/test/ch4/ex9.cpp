/*Create a second version of Progvals.h and ProgVals.cpp that uses a set
instead of a map to manage single-character flags on the command line
(such as -a -b -c etc) and also allows the characters to be ganged up behind
a single hyphen (such as -abc).
*/

#include <gtest/gtest.h>

#include <set>
#include <iostream>
#include <string>

class ProgVals2 : public std::set<char> {
public:
    ProgVals2(std::vector<char>&& availableArgs) {
        mAvailableArgs = availableArgs;
    }

    void parse(int argc, const char* argv[], std::string usage, int offset = 1) {
        for(int i = offset; i < argc; i++) {
            std::string flag(argv[i]);
            if (flag[0] != '-') {
                std::cerr << "Command line error: " <<
                        argv[i] << std::endl << usage << std::endl;
                continue; // Next argument
            }
            std::string arguments = flag.substr(1, flag.length() - 1);
            for (auto c : arguments) {
                if (std::find(mAvailableArgs.begin(), mAvailableArgs.end(), c) == mAvailableArgs.end()) {
                    continue;
                }
                insert(c);
            }
        }
    }

    void print(std::ostream& out = std::cout) {
        out << "Program values:" << std::endl;
        for(iterator it = begin(); it != end(); it++)
            out << *it << std::endl;
    }
private:
    std::vector<char> mAvailableArgs;
};

TEST(ProgVals2, test_happy_path) {
    std::stringstream out;
    ProgVals2 pv({'a', 'b', 'c'});
    const char* args[] = {"test_app", "-a", "-b"};
    pv.parse(3, args, "");
    pv.print(out);
    EXPECT_STREQ("Program values:\n"
                 "a\n"
                 "b\n", out.str().c_str());
}

TEST(ProgVals2, args_are_grouped) {
    std::stringstream out;
    ProgVals2 pv({'a', 'b', 'c'});
    const char* args[] = {"test_app", "-a", "-bc"};
    pv.parse(3, args, "");
    pv.print(out);
    EXPECT_STREQ("Program values:\n"
                 "a\n"
                 "b\n"
                 "c\n", out.str().c_str());
}

TEST(ProgVals2, wrong_args_are_omitted) {
    std::stringstream out;
    ProgVals2 pv({'a', 'b', 'c'});
    const char* args[] = {"test_app", "-d", "ab"};
    pv.parse(3, args, "");
    pv.print(out);
    EXPECT_STREQ("Program values:\n", out.str().c_str());
}
