/*
Change StlShape.cpp so that it uses a deque instead of a vector.
*/

#include <gtest/gtest.h>
#include <deque>
#include <iostream>
using namespace std;

class Shape {
public:
    Shape(std::ostream& output): out{output} { }
    virtual void draw() = 0;
    virtual ~Shape() {}
protected:
    std::ostream& out;
};

class Circle : public Shape {
public:
    Circle(std::ostream& output): Shape(output) {}
    void draw() { out << "Circle::draw\n"; }
    ~Circle() { out << "~Circle\n"; }
};

class Triangle : public Shape {
public:
    Triangle(std::ostream& output): Shape(output) {}
    void draw() { out << "Triangle::draw\n"; }
    ~Triangle() { out << "~Triangle\n"; }
};

class Square : public Shape {
public:
    Square(std::ostream& output): Shape(output) {}
    void draw() { out << "Square::draw\n"; }
    ~Square() { out << "~Square\n"; }
};

typedef std::deque<Shape*> DequeContainer;
typedef std::vector<Shape*> VectorContainer;

template<class T>
int main_func(std::ostream& output) {
    T shapes;
    shapes.push_back(new Circle(output));
    shapes.push_back(new Square(output));
    shapes.push_back(new Triangle(output));
    for(auto i = shapes.begin(); i != shapes.end(); i++)
        (*i)->draw();
    // ... Sometime later:
    for(auto j = shapes.begin(); j != shapes.end(); j++)
        delete *j;
    return 0;
}

TEST(StlShape, test_dequeue) {
    std::stringstream output;
    main_func<DequeContainer>(output);
    EXPECT_STREQ("Circle::draw\n"
                 "Square::draw\n"
                 "Triangle::draw\n"
                 "~Circle\n"
                 "~Square\n"
                 "~Triangle\n", output.str().c_str());
}

TEST(StlShape, test_vector) {
    std::stringstream output;
    main_func<VectorContainer>(output);
    EXPECT_STREQ("Circle::draw\n"
                 "Square::draw\n"
                 "Triangle::draw\n"
                 "~Circle\n"
                 "~Square\n"
                 "~Triangle\n", output.str().c_str());
}
