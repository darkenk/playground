#include "program.hpp"

#include <iostream>
#include <cstdio>
#include <stdexcept>

using namespace std;

Program::Program(std::initializer_list<Shader> shaders):
    mProgram(0)
{
    glGetError();
    vector<GLuint> compiledShaders;
    for (auto s : shaders) {
        compiledShaders.push_back(createShader(s));
    }
    createProgram(compiledShaders);
    for (auto s : compiledShaders) {
        glDeleteShader(s);
    }
}

Program::~Program()
{
    glDeleteProgram(mProgram);
}

GLuint Program::createShader(Shader& shader)
{
    GLuint shaderId = glCreateShader(shader.type);
    const char* str = shader.content.c_str();
    glShaderSource(shaderId, 1, &str, nullptr);
    glCompileShader(shaderId);
    checkCompilationError(shaderId);
    return shaderId;
}

GLuint Program::createProgram(std::vector<GLuint>& shaders)
{
    mProgram = glCreateProgram();
    for (auto shaderId : shaders) {
        glAttachShader(mProgram, shaderId);
    }
    glLinkProgram(mProgram);
}

void Program::checkCompilationError(GLuint shaderId)
{
    GLint success{0};
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (success) {
        return;
    }
    GLint logSize{0};
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
    string errorLog(logSize, ' ');
    glGetShaderInfoLog(shaderId, logSize, &logSize, &errorLog[0]);
    throw logic_error("Compilation error + errorLog");
}

