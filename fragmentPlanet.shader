#version 330 core

in vec3 outNormal;
in vec3 fragPosition;
in vec2 outUV;

uniform sampler2D planetTexture;

out vec4 color;

void main()
{
        vec3 lightColour = vec3(1.0f, 1.0f, 1.0f);
        vec3 cubeColour = vec3(texture(planetTexture, outUV));

        //ambient lighting
        float ambientStrength = 0.20f;
        vec3 ambient_contribution = ambientStrength * lightColour;

        //diffuse lighting
        vec3 light_positions[6];
        light_positions[0] = vec3(0.0f, 30.0f, 0.0f);
        light_positions[1] = vec3(0.0f, -30.0f, 0.0f);
        light_positions[2] = vec3(30.0f, 0.0f, 0.0f);
        light_positions[3] = vec3(-30.0f, 0.0f, 0.0f);
        light_positions[4] = vec3(0.0f, 0.0f, 30.0f);
        light_positions[5] = vec3(0.0f, 0.0f, -30.0f);

        vec3 diffuse_contribution = vec3(0.0f, 0.0f, 0.0f);
        vec3 norm = normalize(outNormal);
        vec3 light_direction;
        float incident_degree;
        int i;

        for(i = 0; i< 6; i++) {
                diffuse_contribution += max(dot(norm, normalize(light_positions[i] - fragPosition)), 0.0f) * lightColour;
        }

        vec3 resultantColour = (ambient_contribution + diffuse_contribution) * cubeColour;
        color = vec4(resultantColour, 1.0f);
}

