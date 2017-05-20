#ifndef DRAWABLEFACTORY_HPP
#define DRAWABLEFACTORY_HPP

#include <memory>
#include "program.hpp"

class DrawableFactory
{
public:
    DrawableFactory();

    template<typename T, typename... Args>
    std::shared_ptr<T> createDrawable(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

private:
    template<typename DRAWABLE, typename PROGRAM>
    void init() {
        DRAWABLE::sProgram = std::make_shared<PROGRAM>();
    }
};

#endif // DRAWABLEFACTORY_HPP
