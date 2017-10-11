/*
Use a stack<int> and build a Fibonacci sequence on the stack. The
program’s command line should take the number of Fibonacci elements
desired, and you should have a loop that looks at the last two elements on
the stack and pushes a new one for every pass through the loop
*/
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stack>
#include <vector>

using ::testing::ContainerEq;

using container = std::vector<uint32_t>;

container convertStackToContainer(std::stack<uint32_t>& stack) {
    container result;
    do {
        result.push_back(stack.top());
        stack.pop();
    } while(not stack.empty());
    std::reverse(result.begin(), result.end());
    return result;
}

container fibonacci(uint32_t number) {
    std::stack<uint32_t> s;
    s.push(0);
    if (number > 0) {
        s.push(1);
    }
    for (uint32_t i = 1; i < number; i++) {
        auto x1 = s.top();
        s.pop();
        auto x2 = s.top();
        s.push(x1);
        s.push(x1 + x2);
    }
    return convertStackToContainer(s);
}

TEST(Fibonacci, fib_returns_zero_for_zero) {
    container expected({0});
    EXPECT_THAT(fibonacci(0), ContainerEq(expected));
}

TEST(Fibonacci, fib_returns_zero_one_for_one) {
    container expected({0, 1});
    EXPECT_THAT(fibonacci(1), ContainerEq(expected));
}

TEST(Fibonacci, fib_returns_zero_one_one_for_two) {
    container expected({0, 1, 1});
    EXPECT_THAT(fibonacci(2), ContainerEq(expected));
}

TEST(Fibonacci, fib_returns_zero_one_one_two_for_three) {
    container expected({0, 1, 1, 2});
    EXPECT_THAT(fibonacci(3), ContainerEq(expected));
}

TEST(Fibonacci, fib_returns_correct_value) {
    container expected({0, 1, 1, 2, 3, 5, 8, 13});
    EXPECT_THAT(fibonacci(7), ContainerEq(expected));
}
