//
//  AsteroidBelt.cpp
//  Asteroid Belt
//
//  Created by Nicholas Sabelli on 2016-06-11.
//  Copyright © 2016 Nicholas Sabelli. All rights reserved.
//

#include "AsteroidBelt.h"

using namespace std;

AsteroidBelt::AsteroidBelt(glm::vec3 centre, float radius, int numberOfrows, glm::vec3 asteroidScale)
{
    _centre = centre;
    _radius = radius;
    _numberOfrows = numberOfrows;
    _asteroidScale = asteroidScale;
    
    BufferInit();
    
    CreateAsteroidBelt();
    
    //Shaders
    _shaderProgram = CreateShaderProgram();
    
    _transformLoc = glGetUniformLocation(_shaderProgram, "model_matrix");
}

void AsteroidBelt::CreateAsteroidBelt()
{
    float radius;
    
    glm::vec3 point;
    
    const int MAX_ASTEROIDS = (360.0f / _radius);
    
    float steps = floor(_numberOfrows / 2);
    
    int row = 0;
    
    for(float zValue = steps * -5.0f; zValue <= steps * 5.0f; zValue += 5.0f)
    {
        radius = _radius;
        
        for (int col = 0; col < 5; col++)
        {
            radius += 5.0f;
            
            bool r0c0c4 = ((row == 0) && ((col == 0) || (col == 4)));
            bool r4c0c4 = ((row == _numberOfrows - 1) && ((col == 0) || (col == 4)));
            
            if(!r0c0c4 && !r4c0c4)
            {
                for(float i = 0.0f; i < 360.0f; i += (rand() % MAX_ASTEROIDS))
                {
                    if((rand() % 100 + 1) > 60) // 40% of the time.
                    {
                        //circle
                        point.x = (float) (_centre.x + radius * cos(glm::radians(i)));
                        point.y = (float) (_centre.y + radius * sin(glm::radians(i)));
                        point.z = zValue;
                        
                        cout << "Asteroid Position: (" << point.x << ", " << point.y << ", " << point.z << ")" << endl;
                        
                        _asteroidPositions.push_back(point);
                        
                        glBindBuffer(GL_ARRAY_BUFFER, _asteroidBeltVBO);
                        glBufferData(GL_ARRAY_BUFFER, _asteroidPositions.size() * sizeof(glm::vec3), &_asteroidPositions[0], GL_STATIC_DRAW);
                        glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind for good measure
                    }
                }
            }
        }
        
        row++;
    }
}

void AsteroidBelt::BufferInit()
{
    glGenVertexArrays(1, &_asteroidBeltVAO);
    glGenBuffers(1, &_cubeVBO);
    glGenBuffers(1, &_asteroidBeltVBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(_asteroidBeltVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, _cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, _asteroidBeltVBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribDivisor(1, 1); //update this attribute per instance, not per vertex.
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind _asteroidBeltVAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    
}

void AsteroidBelt::GameLoop()
{
            glm::mat4 revolverModelMatrix;
    
            //Saturn Ring
            //revolverModelMatrix = glm::rotate(revolverModelMatrix, 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
    
            revolverModelMatrix = glm::rotate(revolverModelMatrix, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    
            //         revolverModelMatrix = glm::rotate(revolverModelMatrix, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
            //        revolverModelMatrix = glm::translate(revolverModelMatrix, cubePositions[1]);
            //        revolverModelMatrix = glm::rotate(revolverModelMatrix, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 0.0f, -1.0f));
            revolverModelMatrix = glm::scale(revolverModelMatrix, _asteroidScale);
    
            glUniformMatrix4fv(_transformLoc, 1, GL_FALSE, glm::value_ptr(revolverModelMatrix));
    
    glUseProgram(_shaderProgram);
    
    glBindVertexArray(_asteroidBeltVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, _asteroidPositions.size());
    glBindVertexArray(0);
}

GLuint AsteroidBelt::CreateShaderProgram()
{
    // Build and compile our shader program
    // Vertex shader
    
    // Read the Vertex Shader code from the file
    string vertex_shader_path = "vertex.shader";
    string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_shader_path, ios::in);
    
    if (VertexShaderStream.is_open()) {
        string Line = "";
        while (getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    else {
        printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
        getchar();
        exit(-1);
    }
    
    // Read the Fragment Shader code from the file
    string fragment_shader_path = "fragment.shader";
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);
    
    if (FragmentShaderStream.is_open()) {
        std::string Line = "";
        while (getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
    else {
        printf("Impossible to open %s. Are you in the right directory?\n", fragment_shader_path.c_str());
        getchar();
        exit(-1);
    }
    
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(fragmentShader, 1, &FragmentSourcePointer, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader); //free up memory
    glDeleteShader(fragmentShader);
    
    //        glUseProgram(shaderProgram);
    
    return shaderProgram;
}


