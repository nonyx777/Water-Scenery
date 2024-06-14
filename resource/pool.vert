#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

uniform mat4 transform;
uniform mat4 model;

out vec3 fragPos;
out vec2 texCoords;
out mat4 model_normal;

void main()
{
    texCoords = aTexCoords;
    fragPos = aPos;
    model_normal = model;

    gl_Position = transform * vec4(aPos, 1.f);
}