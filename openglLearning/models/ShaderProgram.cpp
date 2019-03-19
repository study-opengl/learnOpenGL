//
//  ShaderProgram.cpp
//  createWindow
//
//  Created by Ashoka on 2018/4/29.
//

#include "ShaderProgram.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include "asLog.h"
#include "gtc/type_ptr.hpp"

ShaderProgram::ShaderProgram(const char *vertexPath, const char *fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch(std::ifstream::failure e) {
        asLog("file not success read!");
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    vertex = createShader(&vShaderCode, GL_VERTEX_SHADER);
    fragment = createShader(&fShaderCode, GL_FRAGMENT_SHADER);

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "Program");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void ShaderProgram::use()
{
    glUseProgram(ID);
}

void ShaderProgram::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderProgram::setVec3(const std::string &name, float v0, float v1, float v2) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2);
}

void ShaderProgram::set4Float(const std::string &name, float v0, float v1, float v2, float v3) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2, v3);
}

void ShaderProgram::setMatrix4fv(const std::string &name, glm::mat4 matrix) const
{
    unsigned int uniformLocation = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}

unsigned int ShaderProgram::createShader(const char *const *source, GLenum type)
{
    unsigned int vertex = glCreateShader(type);
    glShaderSource(vertex, 1, source, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    return vertex;
}

void ShaderProgram::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "Program") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            asLog("shader compile error, type: %s", infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            asLog("program link error, type: %s", infoLog);
        }
    }
}
