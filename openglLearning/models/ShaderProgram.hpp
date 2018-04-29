//
//  ShaderProgram.hpp
//  createWindow
//
//  Created by Ashoka on 2018/4/29.
//

#ifndef ShaderProgram_hpp
#define ShaderProgram_hpp

#include <stdio.h>
#include <iostream>
#include "glad.h"

#endif /* ShaderProgram_hpp */

class ShaderProgram {
public:
    unsigned int ID;
    ShaderProgram(const char *vertexPath, const char *fragmentPath);
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void set4Float(const std::string &name, float v0, float v1, float v2, float v3) const;

private:
    unsigned int createShader(const char * const *source, GLenum type);
    void checkCompileErrors(unsigned int shader, std::string type);
};
