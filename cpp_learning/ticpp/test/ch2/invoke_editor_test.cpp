/*
Use your favorite text-searching program (grep, for example) to output the
names (only) of all the files that contain a particular pattern. Redirect the
output into a file. Write a program that uses the contents of that file to
generate a batch file that invokes your editor on each of the files found by
the search program.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "fakefstream.hpp"

using testing::StrEq;

class MockSystemMethod {
public:

    MockSystemMethod() {
        if (sThis != nullptr) {
            throw std::runtime_error("Can be only one MockSystemMethod object");
        }
        sThis = this;
    }

    MOCK_METHOD1(system, int(const char*));

    static int mockSystem(const char* command) {
        return sThis->system(command);
    }

    ~MockSystemMethod() {
        sThis = nullptr;
    }

private:
    static thread_local MockSystemMethod* sThis;
};

thread_local MockSystemMethod* MockSystemMethod::sThis = nullptr;

class InvokeEditor : public ::testing::Test {
public:
    const std::string input_file = "input_file";
    const std::string editor = "test_editor";
    MockSystemMethod system;

protected:
     virtual void TearDown() {
        FakeFstream::clear();
     }
};

template<class istream = std::ifstream, int (*tsystem) (const char* command) = std::system>
void invokeEditor(const std::string& file, const std::string& editor) {
    istream fileList(file);
    std::string fileToEdit;
    while(std::getline(fileList, fileToEdit)) {
        tsystem((editor + " " + fileToEdit).c_str());
    }
}

void (*invokeEditor_test)(const std::string&, const std::string&) = &invokeEditor<FakeIFstream, MockSystemMethod::mockSystem>;

TEST_F(InvokeEditor, invoke_editor_on_one_line_file) {
    FakeIFstream::setFileContent(input_file, "file1");
    EXPECT_CALL(system, system(StrEq(editor + " file1")));
    invokeEditor_test(input_file, editor);
}

TEST_F(InvokeEditor, invoke_editor_on_two_line_file) {
    FakeIFstream::setFileContent(input_file, "file1\nfile2");
    EXPECT_CALL(system, system(StrEq(editor + " file1")));
    EXPECT_CALL(system, system(StrEq(editor + " file2")));
    invokeEditor_test(input_file, editor);
}
