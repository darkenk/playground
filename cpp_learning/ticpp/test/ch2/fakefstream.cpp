#include "fakefstream.hpp"

thread_local std::map<std::string, FakeFstream::FakeFile> FakeFstream::sFakeFiles;
