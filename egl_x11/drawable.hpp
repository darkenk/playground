#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include "program.hpp"
#include <memory>
#include <vector>

class DrawableFactory;

template<typename T>
class Node {
public:
    class Visitor {
    public:
        virtual void beforeGoingDown(T& d) = 0;
        virtual void beforeGoingUp(T& d) = 0;
    };

    class LevelVisitor {
    public:
        virtual void execute(std::vector<std::shared_ptr<T>> childs) = 0;
    };

    Node(): mParent(nullptr) {}
    virtual ~Node() {};

    virtual void addChild(std::shared_ptr<Node<T>> child) {
        mChildren.push_back(child);
        child->mParent = this;
    }

    void traverse(Visitor& v) {
        v.beforeGoingDown(*this);
        for (auto c : mChildren) {
            c->traverse(v);
        }
        v.beforeGoingUp(*this);
    }

    void traverseByLevels(LevelVisitor& v) {
        v.execute(mChildren);
        for (auto c : mChildren) {
            c->traverseByLevels(v);
        }
    }

private:
    std::vector<std::shared_ptr<Node<T>>> mChildren;
    Node<T>* mParent;
};

template<typename ProgramType>
class DKDrawable : public Node<DKDrawable<ProgramType>> {
public:
    DKDrawable() {}
    virtual void draw() = 0;

protected:
    friend DrawableFactory;
    static std::shared_ptr<ProgramType> sProgram;
};

template<typename ProgramType>
std::shared_ptr<ProgramType> DKDrawable<ProgramType>::sProgram;

#endif // DRAWABLE_HPP
