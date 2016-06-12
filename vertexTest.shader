#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 model_matrix_millenium;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    gl_Position = projection_matrix * view_matrix * model_matrix_millenium * vec4(position.x, position.y, position.z, 1.0);
    //gl_Position = vec4(position, 1.0);
}

