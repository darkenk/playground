/* Modify Progvals.h and ProgVals.cpp so that they expect leading hyphens
to distinguish command-line arguments. */

#include <gtest/gtest.h>

//: C07:ProgVals.h
// From "Thinking in C++, 2nd Edition, Volume 2"
// by Bruce Eckel & Chuck Allison, (c) 2001 MindView, Inc.
// Available at www.BruceEckel.com.
// Program values can be changed by command line
#include <map>
#include <iostream>
#include <string>

class ProgVals : public std::map<std::string, std::string> {
public:
    ProgVals(std::string defaults[][2], int sz) {
        for(int i = 0; i < sz; i++)
            insert(make_pair(defaults[i][0], defaults[i][1]));
    }

    void parse(int argc, const char* argv[], std::string usage, int offset = 1) {
        // Parse and apply additional
        // command-line arguments:
        for(int i = offset; i < argc; i++) {
            std::string flag(argv[i]);
            if(flag[0] != '-') {
                std::cerr << "Command line error: " <<
                        argv[i] << std::endl << usage << std::endl;
                continue; // Next argument
            }
            std::string name = flag.substr(1, flag.length() - 1);
            std::string value = argv[++i];
            if(find(name) == end()) {
                std::cerr << name << std::endl << usage << std::endl;
                continue; // Next argument
            }
            operator[](name) = value;
        }
    }

    void print(std::ostream& out = std::cout) {
        out << "Program values:" << std::endl;
        for(iterator it = begin(); it != end(); it++)
            out << (*it).first << " = "
                << (*it).second << std::endl;
    }
};

TEST(ProgVals, test_happy_path) {
    std::stringstream out;
    std::string defaluts[2][2] = { {"a", "default"}, {"b", "default"} };
    const char* args[5] = {"app", "-a", "v1", "-b", "v2"};
    ProgVals pv(defaluts, 2);
    pv.parse(5, args, "");
    pv.print(out);
    EXPECT_STREQ("Program values:\n"
                 "a = v1\n"
                 "b = v2\n", out.str().c_str());
}

TEST(ProgVals, arg_contains_only_hyphen) {
    std::stringstream out;
    std::string defaluts[2][2] = { {"a", "default"}, {"b", "default"} };
    const char* args[5] = {"app", "-", "v1", "-b", "v2"};
    ProgVals pv(defaluts, 2);
    pv.parse(5, args, "");
    pv.print(out);
    EXPECT_STREQ("Program values:\n"
                 "a = default\n"
                 "b = v2\n", out.str().c_str());
}
