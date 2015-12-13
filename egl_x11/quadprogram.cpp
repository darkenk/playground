#include "quadprogram.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

const Shader QuadProgram::sVertexShader = {
    GL_VERTEX_SHADER,
    "attribute vec4 aPosition;   \n"
    "uniform mat4 uModel;          \n"
    "uniform mat4 uViewProj;\n"
    "void main()                 \n"
    "{                           \n"
    "   gl_Position = uViewProj * uModel * aPosition; \n"
    "}                           \n"
};

const Shader QuadProgram::sFragmentShader = {
    GL_FRAGMENT_SHADER,
    "precision mediump float;                   \n"
    "uniform vec4 uColor;\n"
    "void main()                                \n"
    "{                                          \n"
    "  gl_FragColor = uColor; \n"
    "}                                          \n"
};

QuadProgram::QuadProgram():
    Program({sVertexShader, sFragmentShader})
{
    mAPosition = glGetAttribLocation(mProgram, "aPosition");
    mUModel = glGetUniformLocation(mProgram, "uModel");
    mUViewProj = glGetUniformLocation(mProgram, "uViewProj");
    mUColor = glGetUniformLocation(mProgram, "uColor");
    bind();
    glUniformMatrix4fv(mUViewProj, 1, GL_FALSE, glm::value_ptr(glm::mat4()));
    unBind();
}

void QuadProgram::render()
{

}
