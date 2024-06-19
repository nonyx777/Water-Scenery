#version 330 core

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

out vec4 fragColor;

in vec3 fragPos;
in vec2 texCoords;
in vec3 ndc;
in vec3 fragNormal;
uniform vec3 viewPos;

float fresnelFactor;

void main()
{
    //reflection and refraction texture coordinate from homogenous coord to screen space coord
    vec2 screen_space = vec2(ndc.x / 2.f + 0.5f, ndc.y / 2.f + 0.5f);
    vec4 reflectionColor = texture(reflectionTexture, vec2(screen_space.x, 1.f - screen_space.y));
    vec4 refractionColor = texture(refractionTexture, screen_space);

    //fresnel effect
    vec3 viewDir = normalize(viewPos - fragPos);
    fresnelFactor = pow(max(dot(viewDir, fragNormal), 0.f), 2.f);

    fragColor = mix(reflectionColor, refractionColor, fresnelFactor);
    //fragColor = vec4(vec3(fresnelFactor), 1.f);
}