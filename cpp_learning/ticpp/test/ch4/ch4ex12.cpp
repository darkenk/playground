/*Modify BankTeller.cpp so that the policy that decides when a teller is
added or removed is encapsulated inside a class.
Create two classes A and B (feel free to choose more interesting names). */


//: C07:BankTeller.cpp {RunByHand}
// From "Thinking in C++, Volume 2", by Bruce Eckel & Chuck Allison.
// (c) 1995-2004 MindView, Inc. All Rights Reserved.
// See source code use permissions stated in the file 'License.txt',
// distributed with the code package available at www.MindView.net.
// Using a queue and simulated multithreading
// to model a bank teller system.
#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <list>
#include <queue>
using namespace std;

class Customer {
    int serviceTime;
public:
    Customer() : serviceTime(0) {}
    Customer(int tm) : serviceTime(tm) {}
    int getTime() { return serviceTime; }
    void setTime(int newtime) { serviceTime = newtime; }
    friend ostream&
    operator<<(ostream& os, const Customer& c) {
        return os << '[' << c.serviceTime << ']';
    }
};

class Teller {
    queue<Customer>& customers;
    Customer current;
    enum { SLICE = 5 };
    int ttime; // Time left in slice
    bool busy; // Is teller serving a customer?
public:
    Teller(queue<Customer>& cq)
        : customers(cq), ttime(0), busy(false) {}
    Teller(const Teller&) = default;
    Teller& operator=(const Teller& rv) {
        customers = rv.customers;
        current = rv.current;
        ttime = rv.ttime;
        busy = rv.busy;
        return *this;
    }
    bool isBusy() { return busy; }
    void run(bool recursion = false) {
        if(!recursion)
            ttime = SLICE;
        int servtime = current.getTime();
        if(servtime > ttime) {
            servtime -= ttime;
            current.setTime(servtime);
            busy = true; // Still working on current
            return;
        }
        if(servtime < ttime) {
            ttime -= servtime;
            if(!customers.empty()) {
                current = customers.front();
                customers.pop(); // Remove it
                busy = true;
                run(true); // Recurse
            }
            return;
        }
        if(servtime == ttime) {
            // Done with current, set to empty:
            current = Customer(0);
            busy = false;
            return; // No more time in this slice
        }
    }
};

// Inherit to access protected implementation:
class CustomerQ : public queue<Customer> {
public:
    friend ostream&
    operator<<(ostream& os, const CustomerQ& cd) {
        copy(cd.c.begin(), cd.c.end(),
             ostream_iterator<Customer>(os, ""));
        return os;
    }
};

class Policy {
public:
    Policy(CustomerQ& customers, list<Teller>& tellers):
        mCustomers(customers), mTellers(tellers)
    {

    }

    void adjustTellers() {
        // If line is too long, add another teller:
        if(mCustomers.size() / mTellers.size() > 2) {
            mTellers.push_back(Teller(mCustomers));
        }
        // If line is short enough, remove a teller:
        if(mTellers.size() > 1 && mCustomers.size() / mTellers.size() < 2) {
            for(auto i = mTellers.begin(); i != mTellers.end(); i++)
                if(!(*i).isBusy()) {
                    mTellers.erase(i);
                    break; // Out of for loop
                }
        }
    }

private:
    CustomerQ& mCustomers;
    list<Teller>& mTellers;

};

void main_func(std::ostream& out) {
    CustomerQ customers;
    list<Teller> tellers;
    Policy policy(customers, tellers);
    typedef list<Teller>::iterator TellIt;
    tellers.push_back(Teller(customers));
    srand(12); // Seed the random number generator
    clock_t ticks = clock();
    // Run simulation for at least 5 seconds:
    while(clock() < ticks + 5 * CLOCKS_PER_SEC) {
        // Add a random number of customers to the
        // queue, with random service times:
        for(int i = 0; i < rand() % 5; i++)
            customers.push(Customer(rand() % 15 + 1));
        out << '{' << tellers.size() << '}'
             << customers << endl;
        // Have the tellers service the queue:
        for(TellIt i = tellers.begin();
            i != tellers.end(); i++)
            (*i).run();
        out << '{' << tellers.size() << '}'
             << customers << endl;
        policy.adjustTellers();
    }
}
// TODO: how to test this case?
TEST(ch4ex12, main_func) {
    std::stringstream out;
    main_func(std::cout);
    EXPECT_EQ("", out.str());
}
