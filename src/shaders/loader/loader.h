#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>

#include <glad/glad.h>

class ShaderLoader
{
public:
    static std::string ReadTextFile(const std::string &path);
    static GLuint CompileShader(GLenum type, const char *source);
    static GLuint CreateShaderProgram(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
};

#endif
