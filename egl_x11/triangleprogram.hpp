#ifndef TRIANGLEPROGRAM_HPP
#define TRIANGLEPROGRAM_HPP

#include "program.hpp"
#include <glm/glm.hpp>

class TriangleProgram : public Program
{
public:
    TriangleProgram();
    virtual void render();

private:
    static const Shader sVertexShader;
    static const Shader sFragmentShader;
    static const float sVertices[];

    GLuint mAPosition;
    GLuint mUMVP;
    glm::mat4 mMVP;
};

#endif // TRIANGLEPROGRAM_HPP
