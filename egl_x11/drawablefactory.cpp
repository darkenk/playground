#include "drawablefactory.hpp"

#include "quaddrawable.hpp"
#include "quadtexture.hpp"

DrawableFactory::DrawableFactory()
{
    init<QuadDrawable, QuadProgram>();
    init<QuadTexture, QuadProgram>();
}
