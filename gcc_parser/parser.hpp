#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <vector>

class File
{
public:
    File(std::string& name, std::string& defines, std::string includes) :
        mName{name}, mDefines{defines}, mIncludes{includes} {}
    std::string mName;
    std::string mDefines;
    std::string mIncludes;
    friend std::ostream& operator<<(std::ostream& o, const File& f) {
        return o << f.mName << " D:" << f.mDefines << " I:" << f.mIncludes;
    }
};

class ParserException : public std::exception
{
public:
    ParserException(const std::string& msg) noexcept : mMessage{msg} {}
    virtual const char* what() const noexcept;

private:
    std::string mMessage;
};

class Parser
{
public:
    Parser(std::istream& input);
    void run();
    std::vector<File>& result();
    std::string generateQtCreatorConfigFileContent();
    std::string generateQtCreatorIncludeFileContent();

private:
    std::istream& mInputStream;
    std::vector<File> mResult;
    bool isCompilerLine(const std::string& line);
    void parseLine(const std::string& line);
    std::string parseFileName(const std::string& line);
    std::string parseDefines(const std::string& line);
    std::string parseIncludes(const std::string& line);
    std::string prepareQtCreatorDefine(const std::string& d);
};

#endif // PARSER_HPP
