#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

uniform mat4 transform;
uniform mat4 model;

out vec3 fragPos;
out vec2 texCoords;
out mat4 model_normal;

uniform vec4 clipPlane;

void main()
{
    texCoords = aTexCoords;
    fragPos = vec3(model * vec4(aPos, 1.f));
    model_normal = model;

    vec4 position = model * vec4(aPos, 1.f);
    gl_ClipDistance[0] = dot(position, clipPlane);
    
    gl_Position = transform * vec4(aPos, 1.f);
}