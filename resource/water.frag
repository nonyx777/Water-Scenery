#version 330 core

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

out vec4 fragColor;

in vec3 fragPos;
in vec2 texCoords;
in vec3 ndc;

void main()
{
    vec2 screen_space = ndc.xy / 2.f + 0.5f;

    vec4 reflectionColor = texture(reflectionTexture, vec2(screen_space.x, -screen_space.y));
    vec4 refractionColor = texture(refractionTexture, screen_space);

    fragColor = mix(reflectionColor, refractionColor, 0.5f);
}