#include "gtest/gtest.h"

#include <thread>

using namespace std;

TEST(thread, test1) {
    thread t([] {int i = 2 + 3;});
    EXPECT_TRUE(0 == 1);
}
