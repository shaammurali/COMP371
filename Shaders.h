#ifndef SHADERS_H
#define SHADERS_H

#include <string>

#include "GL/glew.h"

std::string readShaderFile(std::string path);
GLuint compileShader(std::string shaderType, std::string shaderCode);
GLuint linkShaders(GLuint vertexShader, GLuint fragmentShader);

#endif /* SHADERS_H */

