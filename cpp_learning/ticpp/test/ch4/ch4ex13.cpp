/* Create two classes A and B (feel free to choose more interesting names).
Create a multimap<A, B> and fill it with key-value pairs, ensuring that
there are some duplicate keys. Use equal_range( ) to discover and print a
range of objects with duplicate keys. Note you may have to add some
functions in A and/or B to make this program work.
Perform the above exercise for a multiset<A>.
*/

#include <gtest/gtest.h>
#include <map>

class Key {
public:
    Key(int id): mId(id) {}

    bool operator<(const Key& other) const {
        return mId < other.mId;
    }

private:
    int mId;
};
class Value {
public:
    Value(int v): mValue(v) {}

    int get() {
        return mValue;
    }
private:
    int mValue;
};

TEST(ch4ex13, calculate_distance)
{
    std::multimap<Key, Value> map;
    map.insert(std::make_pair(Key(1), Value(1)));
    map.insert(std::make_pair(Key(2), Value(2)));
    map.insert(std::make_pair(Key(1), Value(3)));
    auto ret = map.equal_range(Key(1));
    EXPECT_EQ(2, std::distance(ret.first, ret.second));
}

TEST(ch4ex13, iterate_over_values)
{
    std::multimap<Key, Value> map;
    map.insert(std::make_pair(Key(1), Value(1)));
    map.insert(std::make_pair(Key(2), Value(2)));
    map.insert(std::make_pair(Key(1), Value(3)));
    auto ret = map.equal_range(Key(1));
    EXPECT_EQ(1, ret.first->second.get());
    EXPECT_EQ(3, (++ret.first)->second.get());
}

TEST(ch4ex13, calculate_distance_for_multiset)
{
    std::multiset<Key> set;
    set.insert(Key(1));
    set.insert(Key(2));
    set.insert(Key(1));
    auto ret = set.equal_range(Key(1));
    EXPECT_EQ(2, std::distance(ret.first, ret.second));
}
