#version 330 core

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvTexture;
uniform float dudvTime;

out vec4 fragColor;

in vec3 fragPos;
in vec2 texCoords;
in vec3 ndc;
in vec3 fragNormal;
uniform vec3 viewPos;

float fresnelFactor;
float dudvInfluence = 0.01f;

void main()
{
    //reflection and refraction texture coordinate from homogenous coord to screen space coord
    vec2 screen_space = vec2(ndc.x / 2.f + 0.5f, ndc.y / 2.f + 0.5f);
    vec2 offset1 = texture(dudvTexture, texCoords + vec2(dudvTime, 0.f)).rg * 2.f - 1.f;
    vec2 offset2 = texture(dudvTexture, texCoords + vec2(-dudvTime, dudvTime)).rg * 2.f - 1.f;
    screen_space = screen_space + (offset1 + offset2) * dudvInfluence;

    vec2 reflectionCoord = vec2(screen_space.x, 1.f - screen_space.y);
    vec2 refractionCoord = screen_space;

    clamp(reflectionCoord.x, 0.001f, 0.999f);
    clamp(reflectionCoord.y, -0.999f, -0.001f);
    clamp(refractionCoord, 0.001f, 0.999f);

    vec4 reflectionColor = texture(reflectionTexture, reflectionCoord);
    vec4 refractionColor = texture(refractionTexture, refractionCoord);

    //fresnel effect
    vec3 viewDir = normalize(viewPos - fragPos);
    fresnelFactor = pow(max(dot(viewDir, fragNormal), 0.f), 2.f);

    fragColor = mix(reflectionColor, refractionColor, fresnelFactor);
    fragColor = mix(fragColor, vec4(0.f, 0.3f, 0.4f, 1.f), 0.2f);
}