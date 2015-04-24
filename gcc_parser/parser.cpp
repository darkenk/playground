#include "parser.hpp"

#include <regex>
#include <iterator>

using namespace std;

Parser::Parser(istream& input) :
    mInputStream{input}
{
}

void Parser::run()
{
    string option;
    while (getline(mInputStream, option)) {
        if (isCompilerLine(option)) {
            parseLine(option);
        }
    }
}

std::vector<File>& Parser::result()
{
    return mResult;
}

string Parser::generateQtCreatorConfigFileContent()
{
    string s;
    for (auto f : mResult) {
        s += f.mName + '\n';
        s += f.mDefines + '\n';
    }
    return s;
}

string Parser::generateQtCreatorIncludeFileContent()
{
    string s;
    for (auto f : mResult) {
        s += f.mName + '\n';
        s += f.mIncludes + '\n';
    }
    return s;
}

void Parser::parseLine(const string& line)
{
    try {
        string f = parseFileName(line);
        string d = parseDefines(line);
        string i = parseIncludes(line);
        mResult.push_back(File(f, d, i));
    } catch(ParserException& e) {
        cout << "Could not parse " << e.what() << '\n';
    }
}

string Parser::parseFileName(const string& line)
{
    static const regex r{"[^ ]*\\.(cc|cpp|c)"};
    auto it = sregex_iterator(line.begin(), line.end(), r);
    auto end = sregex_iterator();
    if (it == end) {
        throw ParserException(line);
    }
    string s;
    s += "[" + it->str() + "]";
    return s;
}

string Parser::parseDefines(const string& line)
{
    static const regex r{"-D[^ ]*"};
    auto it = sregex_iterator(line.begin(), line.end(), r);
    auto end = sregex_iterator();
    string s{};
    while (it != end) {
        s += prepareQtCreatorDefine(it->str()) + "\n";
        ++it;
    }
    if (not s.empty()) {
        s.erase(s.end()-1);
    }
    return s;
}

string Parser::parseIncludes(const string& line)
{
    static const regex r{"(-I|-isystem) [^ ]*"};
    auto it = sregex_iterator(line.begin(), line.end(), r);
    auto end = sregex_iterator();
    string s{};
    while (it != end) {
        s += it->str().substr(it->str().find(" ")+1) + '\n';
        ++it;
    }
    if (not s.empty()) {
        s.erase(s.end()-1);
    }
    return s;
}

string Parser::prepareQtCreatorDefine(const string& d)
{
    static const string define{"#define "};
    string s{d.begin()+2, d.end()};
    auto p = s.find_first_of("=");
    if (p != string::npos) {
        s.replace(p, 1, " ");
    }
    return define + s;
}

bool Parser::isCompilerLine(const string& line)
{
    static const regex r{".*(gcc|g++|clang|clang++) .*"};
    return regex_match(line, r);
}

const char* ParserException::what() const noexcept
{
    return mMessage.c_str();
}
