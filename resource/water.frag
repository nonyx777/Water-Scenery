#version 330 core

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

out vec4 fragColor;

in vec3 fragPos;
in vec2 texCoords;

vec4 reflectionColor = texture(reflectionTexture, texCoords);
vec4 refractionColor = texture(refractionTexture, texCoords);

void main()
{
    fragColor = mix(reflectionColor, refractionColor, 0.5f);
}