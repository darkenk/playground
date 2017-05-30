#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <fstream>
#include <map>

class FakeOFstream : public std::stringstream {
public:
    FakeOFstream(const std::string& fileName,
           ios_base::openmode mode = ios_base::out|ios_base::trunc) {
        mFileName = fileName;
        sFileContent[mFileName];
        if (mode | ios_base::trunc) {
            sFileContent[mFileName].clear();
        } else {
            str(sFileContent[mFileName]);
        }
    }
    ~FakeOFstream() {
        sFileContent[mFileName].append(str());
    }

    static std::string& getFileContent(const std::string& fileName) {
        return sFileContent[fileName];
    }

private:
    std::string mFileName;
    thread_local static std::map<std::string, std::string> sFileContent;
};

class FakeIFstream : public std::stringstream {
public:
    FakeIFstream(const std::string& fileName, ios_base::openmode /*mode*/ = ios_base::in) {
        *this << sFileContent[fileName];
    }
    ~FakeIFstream() {
    }

    static void setFileContent(const std::string& fileName, const std::string& content) {
        sFileContent[fileName] = content;
    }

private:
    thread_local static std::map<std::string, std::string> sFileContent;
};

thread_local std::map<std::string, std::string> FakeOFstream::sFileContent;
thread_local std::map<std::string, std::string> FakeIFstream::sFileContent;

template<class outstream = std::ofstream, class instream = std::ifstream>
void toupper(const std::string& inFile, const std::string& outFile) {
    instream in(inFile);
    outstream out(outFile);
    std::transform(std::istream_iterator<char>(in), std::istream_iterator<char>(),
                   std::ostream_iterator<char>(out), [](char c) { return std::toupper(c); });
}

void (*toupper_test)(const std::string&, const std::string&) = &toupper<FakeOFstream, FakeIFstream>;

//TEST(Toupper, does_it_compile_with_fstream) {
    template void toupper(const std::string&, const std::string&);
//}

TEST(Toupper, create_empty_file_for_empty_file) {
    toupper_test("empty_file", "new_file");
    EXPECT_EQ(FakeOFstream::getFileContent("new_file"), "");
}

TEST(Toupper, copy_upper_case_file) {
    FakeIFstream::setFileContent("input_file", "AA");
    toupper_test("input_file", "out_file");
    EXPECT_EQ(FakeOFstream::getFileContent("out_file"), "AA");
}

TEST(Toupper, change_to_upper_case_file_aa) {
    FakeIFstream::setFileContent("input_file", "aa");
    toupper_test("input_file", "out_file");
    EXPECT_EQ(FakeOFstream::getFileContent("out_file"), "AA");
}

TEST(Toupper, change_to_upper_case_file_aA) {
    FakeIFstream::setFileContent("input_file", "aA");
    toupper_test("input_file", "out_file");
    EXPECT_EQ(FakeOFstream::getFileContent("out_file"), "AA");
}
