#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

uniform mat4 transform;

out vec3 fragPos;
out vec2 texCoords;

void main()
{
    texCoords = aTexCoords;
    fragPos = aPos;

    gl_Position = transform * vec4(aPos, 1.f);
}