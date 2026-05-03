#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <glad/glad.h>

class ShaderLoader
{
public:
    static std::string readTextFile(const std::string &path);
    static GLuint compileShader(GLenum type, const char *source);
    static GLuint createShaderProgram(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
};

#endif
