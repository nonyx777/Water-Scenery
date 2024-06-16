#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec3 fragPos;
out vec2 texCoords;

uniform mat4 transform;
uniform mat4 model;

uniform vec4 clipPlane;

out vec3 ndc;

void main()
{
    texCoords = aTexCoords;
    fragPos = aPos;

    vec4 clipSpace = transform * vec4(aPos, 1.f);
    ndc = clipSpace.xyz / clipSpace.w;

    vec4 position = model * vec4(aPos, 1.f);
    gl_ClipDistance[0] = dot(position, clipPlane);

    gl_Position = transform * vec4(aPos, 1.f);
}