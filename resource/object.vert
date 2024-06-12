#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 texCoords;
out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 transform;
uniform mat4 model;

void main()
{
    texCoords = aTexCoords;
    fragPos = aPos;
    fragNormal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = transform * vec4(aPos, 1.f);
}