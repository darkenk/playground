#include "drawablefactory.hpp"

#include "quaddrawable.hpp"

DrawableFactory::DrawableFactory()
{
    init<QuadDrawable, QuadProgram>();
}
