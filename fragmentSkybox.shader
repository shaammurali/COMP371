#version 330 core

in vec3 outNormal;
in vec3 fragPosition;
in vec2 outUV;
in vec3 skyboxCoord;

out vec4 color;

uniform samplerCube skybox;

uniform bool drawingSkybox;

void main()
{
        color = texture(skybox, skyboxCoord);
}
