#include "quaddrawable.hpp"
#include <glm/gtc/type_ptr.hpp>

const QuadDrawable::Vertex QuadDrawable::sVertices[] = {
    Vertex(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
    Vertex(glm::vec4(50.0f, 0.0f, 0.0f, 1.0f)),
    Vertex(glm::vec4(0.0f, 50.0f, 0.0f, 1.0f)),
    Vertex(glm::vec4(50.0f, 50.0f, 0.0f, 1.0f)),
};

void QuadDrawable::draw()
{
    sProgram->bind();
    glEnableVertexAttribArray(sProgram->getAttribPosition());
    glVertexAttribPointer(sProgram->getAttribPosition(), 4, GL_FLOAT, GL_FALSE, 0, sVertices);
    glUniformMatrix4fv(sProgram->getUniformModel(), 1, GL_FALSE, glm::value_ptr(mModel));
    glUniform4fv(sProgram->getUniformColor(), 1, glm::value_ptr(mColor));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
