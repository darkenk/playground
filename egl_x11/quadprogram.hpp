#ifndef QUADPROGRAM_HPP
#define QUADPROGRAM_HPP

#include "program.hpp"

class QuadProgram : public Program
{
public:
    QuadProgram();
    virtual void render();

    GLuint getAttribPosition() { return mAPosition; }
    GLuint getUniformModel() { return mUModel; }
    GLuint getUniformViewProj() { return mUViewProj; }
    GLuint getUniformColor() { return mUColor; }

private:
    static const Shader sVertexShader;
    static const Shader sFragmentShader;

    GLuint mAPosition;
    GLuint mUModel;
    GLuint mUViewProj;
    GLuint mUColor;
};

#endif // QUADPROGRAM_HPP
