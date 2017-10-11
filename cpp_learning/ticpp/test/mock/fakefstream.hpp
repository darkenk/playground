#ifndef FAKEFSTREAM_HPP
#define FAKEFSTREAM_HPP

#include <fstream>
#include <string>
#include <sstream>
#include <map>

class FakeFstream : public std::stringstream {
public:
    FakeFstream(const std::string& fileName) {
        auto state = sFakeFiles.find(fileName);
        mFileName = fileName;
        if (state != sFakeFiles.end()) {
            setstate(state->second.state);
        }
    }

    virtual ~FakeFstream() {}

    static void setNoFile(const std::string& fileName) {
        sFakeFiles[fileName].state = failbit;
    }

    static void clear() {
        sFakeFiles.clear();
    }

    static std::string& getFileContent(const std::string& fileName) {
        return sFakeFiles[fileName].content;
    }

    static void setFileContent(const std::string& fileName, const std::string& content) {
        sFakeFiles[fileName].content = content;
    }

protected:
    std::string mFileName;
    struct FakeFile {
        std::string content;
        iostate state;
    };
    thread_local static std::map<std::string, FakeFile> sFakeFiles;
};

class FakeOFstream : public FakeFstream {
public:
    FakeOFstream(const std::string& fileName,
           ios_base::openmode mode = ios_base::out | ios_base::trunc) : FakeFstream(fileName) {
        if (mode & ios_base::trunc) {
            sFakeFiles[mFileName].content.clear();
        } else {
            *this << sFakeFiles[mFileName].content;
        }
    }
    ~FakeOFstream() {
        sFakeFiles[mFileName].content = str();
    }

private:
};

class FakeIFstream : public FakeFstream {
public:
    FakeIFstream(const std::string& fileName, ios_base::openmode /*mode*/ = ios_base::in) :
        FakeFstream(fileName) {
        *this << sFakeFiles[fileName].content;
    }

private:
};

#endif // FAKEFSTREAM_HPP

