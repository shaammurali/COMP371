//
//  AsteroidBelt.h
//  Asteroid Belt
//
//  Created by Nicholas Sabelli on 2016-06-14.
//  Copyright © 2016 Nicholas Sabelli. All rights reserved.
//

#include <GL/glew.h> // include GL Extension Wrangler
#include <GLFW/glfw3.h> // include GLFW helper library
#include <glm/glm.hpp> //Include GLM library.
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>

using namespace std;

#ifndef AsteroidBelt_h
#define AsteroidBelt_h

class AsteroidBelt
{
private:
    int _numberOfrows;
    
    float _radius;
    
    glm::vec3 _centre;
    glm::vec3 _asteroidScale;
    
    vector<glm::vec3> _asteroidPositions;
    
    GLuint _shaderProgram;
    GLuint _asteroidBeltVAO, _cubeVBO, _asteroidBeltVBO;
    GLuint _transformLoc;
    
    GLfloat cubeVertices[36*3] = {
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };
    
    void CreateAsteroidBelt();
    void BufferInit();
    GLuint CreateShaderProgram();
    
public:
    AsteroidBelt(glm::vec3, float, int, glm::vec3);
    void GameLoop();
    GLuint GetShader()
    {
        return _shaderProgram;
    }
};

#endif /* AsteroidBelt_h */
