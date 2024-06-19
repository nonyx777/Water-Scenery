#version 330 core

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

out vec4 fragColor;

in vec3 fragPos;
in vec2 texCoords;

vec3 fragNormal;

void main()
{
    fragNormal = vec3(0.f, 1.f, 0.f);
    fragColor = texture(diffuseTexture, texCoords);
}