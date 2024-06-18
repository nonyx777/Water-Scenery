#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec3 fragPos;
out vec2 texCoords;
out vec3 ndc;
out vec3 fragNormal;

uniform mat4 transform;
uniform mat4 model;
uniform vec4 clipPlane;

void main()
{
    fragNormal = vec3(0.f, 1.f, 0.f);

    texCoords = aTexCoords;
    fragPos = vec3(model * vec4(aPos, 1.f));

    vec4 clipSpace = transform * vec4(aPos, 1.f);
    ndc = clipSpace.xyz / clipSpace.w;

    vec4 position = model * vec4(aPos, 1.f);
    gl_ClipDistance[0] = dot(position, clipPlane);


    gl_Position = transform * vec4(aPos, 1.f);
}