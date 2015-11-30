#include "triangleprogram.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

const Shader TriangleProgram::sVertexShader = {
    GL_VERTEX_SHADER,
    "attribute vec4 vPosition;   \n"
    "uniform mat4 uMVP;          \n"
    "void main()                 \n"
    "{                           \n"
    "   gl_Position = uMVP * vPosition; \n"
    "}                           \n"
};

const Shader TriangleProgram::sFragmentShader = {
    GL_FRAGMENT_SHADER,
    "precision mediump float;                   \n"
    "void main()                                \n"
    "{                                          \n"
    "  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); \n"
    "}                                          \n"
};

const float TriangleProgram::sVertices[] = { 0.0f,  0.43f, 0.0f,
                                            -0.5f, -0.43f, 0.0f,
                                             0.5f, -0.43f, 0.0f};

TriangleProgram::TriangleProgram():
    Program({sVertexShader, sFragmentShader})
{
    mAPosition = glGetAttribLocation(mProgram, "vPosition");
    mUMVP = glGetUniformLocation(mProgram, "uMVP");
}

void TriangleProgram::render()
{
    glUseProgram(mProgram);
    glVertexAttribPointer(mAPosition, 3, GL_FLOAT, GL_FALSE, 0, sVertices);
    glEnableVertexAttribArray(mAPosition);
    glUniformMatrix4fv(mUMVP, 1, GL_FALSE, glm::value_ptr(mMVP));
    mMVP = glm::rotate(mMVP, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    glDrawArrays(GL_TRIANGLES, 0, 3);
}
