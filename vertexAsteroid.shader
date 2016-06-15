#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 displacement;

uniform mat4 model_matrix_asteroid;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    gl_Position = projection_matrix * view_matrix * model_matrix_asteroid * vec4(position + displacement, 1.0f);
}

