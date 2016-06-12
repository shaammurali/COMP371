#include <iostream>
#include <string>
#include <fstream>

#include "GL/glew.h"

#include "Shaders.h"

GLint success;
GLchar infoLog[512];

// abstracts all code required for reading, compiling and linking shaders.

std::string readShaderFile(std::string path) {
    std::string shaderCode;
    std::ifstream shaderStream(path, std::ios::in);
    if (shaderStream.is_open()) {
        std::string line = "";
        while (getline(shaderStream, line))
            shaderCode += "\n" + line;
        shaderStream.close();
        return shaderCode;
    } else {
        printf("Impossible to open %s. Are you in the right directory ?\n", path.c_str());
        getchar();
        exit(-1);
    }
}


// Possible values for shaderType are "vertex" or "fragment", ignoring case
GLuint compileShader(std::string shaderType, std::string shaderCode) {
    GLuint shader;

    if (shaderType == "vertex") {
        shader = glCreateShader(GL_VERTEX_SHADER);
    } else if (shaderType == "fragment") {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    } else {
        std::cout << "ERROR: Impossible to compile shader type " << shaderType << std::endl;
        exit(-1);
    }
    char const * sourcePointer = shaderCode.c_str();
    glShaderSource(shader, 1, &sourcePointer, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR: " << shaderType << " shader compilation failed \n" << infoLog << std::endl;
        exit(-1);
    }
    return shader;
}

GLuint linkShaders(GLuint vertexShader, GLuint fragmentShader) {
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR: shader program linking failed\n" << infoLog << std::endl;
        exit(-1);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

