/* Rewrite Ring.cpp so it uses a deque instead of a list for its underlying
implementation. */

//: C04:Ring.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 2000
// Copyright notice in Copyright.txt
// Making a "ring" data structure from the STL
#include <gtest/gtest.h>
#include <iostream>
#include <queue>
#include <string>

using namespace std;

template<class T>
class Ring {
    deque<T> lst;
public:
    // Declaration necessary so the following
    // 'friend' statement sees this 'iterator'
    // instead of std::iterator:
    class iterator;
    friend class iterator;
    class iterator : public std::iterator<
            std::bidirectional_iterator_tag,T,ptrdiff_t>{
        typename deque<T>::iterator it;
        deque<T>* r;
    public:
        // "typename" necessary to resolve nesting:
        iterator(deque<T>& _lst,
                 const typename deque<T>::iterator& i)
            : it(i), r(&_lst) {}
        bool operator==(const iterator& x) const {
            return it == x.it;
        }
        bool operator!=(const iterator& x) const {
            return !(*this == x);
        }
        typename deque<T>::reference operator*() const {
            return *it;
        }
        iterator& operator++() {
            ++it;
            if(it == r->end())
                it = r->begin();
            return *this;
        }
        iterator operator++(int) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }
        iterator& operator--() {
            if(it == r->begin())
                it = r->end();
            --it;
            return *this;
        }
        iterator operator--(int) {
            iterator tmp = *this;
            --*this;
            return tmp;
        }
        iterator insert(const T& x){
            return iterator(*r, r->insert(it, x));
        }
        iterator erase() {
            return iterator(*r, r->erase(it));
        }
    };
    void push_back(const T& x) {
        lst.push_back(x);
    }
    iterator begin() {
        return iterator(lst, lst.begin());
    }
    std::size_t size() { return lst.size(); }
};

void main_func(std::ostream& out) {
    Ring<string> rs;
    rs.push_back("one");
    rs.push_back("two");
    rs.push_back("three");
    rs.push_back("four");
    rs.push_back("five");
    Ring<string>::iterator it = rs.begin();
    it++; it++;
    it.insert("six");
    it = rs.begin();
    // Twice around the ring:
    for(std::size_t i = 0; i < rs.size() * 2; i++)
        out << *it++ << endl;
}

TEST(ch4ex16, test1)
{
    std::stringstream out;
    main_func(out);
    EXPECT_STREQ("one\ntwo\nsix\nthree\nfour\nfive\none\ntwo\nsix\nthree\nfour\nfive\n", out.str().c_str());
}
