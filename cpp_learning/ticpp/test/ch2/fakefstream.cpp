#include "fakefstream.hpp"

thread_local std::map<std::string, std::string> FakeFstream::sFileContent;
thread_local std::map<std::string, FakeFstream::iostate> FakeFstream::sState;
