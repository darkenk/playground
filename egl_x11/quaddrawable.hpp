#ifndef QUADDRAWABLE_HPP
#define QUADDRAWABLE_HPP

#include "drawable.hpp"
#include "quadprogram.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

class QuadDrawable : public DKDrawable<QuadProgram>
{
public:
    QuadDrawable(const glm::vec4& color): mColor(color), mUpdateModel(false) {
        static unsigned int id = 0;
        mId = id++;
    }

    virtual void draw();

    void setViewProj(const glm::mat4& viewProj) {
        sProgram->bind();
        glUniformMatrix4fv(sProgram->getUniformViewProj(), 1, GL_FALSE, glm::value_ptr(viewProj));
    }

    void setPosition(const glm::vec3& v) {
        mPosition = v;
        mUpdateModel = true;
    }

    void setZ(const float z) {
        mPosition.z = z;
        mUpdateModel = true;
    }

    void updateModel() {
        if (not mUpdateModel) {
            return;
        }
        mUpdateModel = false;
        glm::mat4 m;
        mModel = glm::translate(m, mPosition);
    }

    int id() {
        return mId;
    }

private:
    struct Vertex {
        Vertex(glm::vec4 _pos): pos(_pos) {}
        glm::vec4 pos;
    };

    static const Vertex sVertices[];

    bool mUpdateModel;
    glm::mat4 mModel;
    glm::vec3 mPosition;
    glm::vec4 mColor;
    unsigned int mId;
};

#endif // QUADDRAWABLE_HPP
