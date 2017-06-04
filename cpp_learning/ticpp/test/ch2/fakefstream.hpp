#ifndef FAKEFSTREAM_HPP
#define FAKEFSTREAM_HPP

#include <fstream>
#include <string>
#include <sstream>
#include <map>

class FakeFstream : public std::stringstream {
public:
    FakeFstream(const std::string& fileName) {
        mFileName = fileName;
        if (sState.find(fileName) != sState.end()) {
            setstate(sState[fileName]);
        }
    }

    virtual ~FakeFstream() {}

    static void setNoFile(const std::string& fileName) {
        sState[fileName] = failbit;
    }

    static void clear() {
        sFileContent.clear();
        sState.clear();
    }

    static std::string& getFileContent(const std::string& fileName) {
        return sFileContent[fileName];
    }

    static void setFileContent(const std::string& fileName, const std::string& content) {
        sFileContent[fileName] = content;
    }

protected:
    std::string mFileName;
    thread_local static std::map<std::string, std::string> sFileContent;
    thread_local static std::map<std::string, iostate> sState;
};

class FakeOFstream : public FakeFstream {
public:
    FakeOFstream(const std::string& fileName,
           ios_base::openmode mode = ios_base::out|ios_base::trunc) : FakeFstream(fileName) {
        if (mode | ios_base::trunc) {
            sFileContent[mFileName].clear();
        } else {
            str(sFileContent[mFileName]);
        }
    }
    ~FakeOFstream() {
        sFileContent[mFileName].append(str());
    }

private:
};

class FakeIFstream : public FakeFstream {
public:
    FakeIFstream(const std::string& fileName, ios_base::openmode /*mode*/ = ios_base::in) :
        FakeFstream(fileName) {
        *this << sFileContent[fileName];
    }

private:
};

#endif // FAKEFSTREAM_HPP

