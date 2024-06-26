#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec3 fragPos;
out vec2 texCoords;

uniform mat4 transform;
uniform mat4 model;

uniform vec4 clipPlane;

void main()
{
    texCoords = aTexCoords;
    fragPos = vec3(model * vec4(aPos, 1.f));
    gl_Position = transform * vec4(aPos, 1.f);
    gl_ClipDistance[0] = dot(model * vec4(aPos, 1.f), clipPlane);
}