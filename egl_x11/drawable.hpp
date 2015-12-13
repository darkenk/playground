#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include "program.hpp"
#include <memory>
#include <vector>

class DrawableFactory;

class Node;

class Node {
public:
    class Visitor {
    public:
        virtual void beforeGoingDown(Node& d) = 0;
        virtual void beforeGoingUp(Node& d) = 0;
    };

    class LevelVisitor {
    public:
        virtual void execute(std::vector<std::shared_ptr<Node>> childs) = 0;
    };

    Node(): mParent(nullptr) {}
    virtual ~Node() {};

    void addChild(std::shared_ptr<Node> child) {
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
    std::vector<std::shared_ptr<Node>> mChildren;
    Node* mParent;
};

template<typename ProgramType>
class DKDrawable : public Node {
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
