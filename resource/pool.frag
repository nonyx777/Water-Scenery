#version 330 core

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

out vec4 fragColor;

//in vec3 fragPos;
in vec2 texCoords;

void main()
{
    vec4 color = texture(diffuseMap, texCoords);
    fragColor = color;
}