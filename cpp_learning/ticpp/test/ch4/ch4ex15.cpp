/*Create a class that has an operator< and an ostream& operator<<. The
class should contain a priority number. Create a generator for your class that
makes a random priority number. Fill a priority_queue using your
generator, then pull the elements out to show they are in the proper order.*/

#include <gtest/gtest.h>
#include <queue>

class Priority {
public:
    Priority(int prio): mPriority(prio) {}

    bool operator<(const Priority& p) const {
        return mPriority < p.mPriority;
    }

private:
    int mPriority;
    friend std::ostream& operator<<(std::ostream& out, const Priority& p);
};

std::ostream& operator<<(std::ostream& out, const Priority& p) {
    return out << p.mPriority;
}

TEST(ch4ex15, test1)
{
    std::priority_queue<Priority> queue;
    queue.push(1);
    queue.push(2);
    queue.push(0);
    queue.push(13);
    queue.push(9);
    std::stringstream stream;
    while (not queue.empty()) {
        stream << queue.top() << " ";
        queue.pop();
    }
    EXPECT_STREQ("13 9 2 1 0 ", stream.str().c_str());
}

