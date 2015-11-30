#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <GLES2/gl2.h>
#include <string>
#include <vector>
#include <initializer_list>

struct Shader
{
    Shader(GLuint _type, std::string _content): type{_type}, content{_content} {}
    const GLuint type;
    const std::string content;
};

class Program
{
public:
    Program(std::initializer_list<Shader> shaders);
    virtual ~Program();

    virtual void render() = 0;

    void bind() {
        glUseProgram(mProgram);
    }

    void unBind() {
        glUseProgram(mProgram);
    }

protected:
    GLuint mProgram;

private:
    GLuint createShader(Shader& shader);
    GLuint createProgram(std::vector<GLuint>& shaders);
    void checkCompilationError(GLuint shaderId);
};

#endif // PROGRAM_HPP
