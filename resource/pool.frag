#version 330 core

struct Light
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct Material
{
    float shininess;
};

uniform Light light;
uniform Material material;
uniform vec3 viewPos;


uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

out vec4 fragColor;

in vec3 fragPos;
in vec2 texCoords;
in mat4 model_normal;

vec3 fragNormal;
vec4 sampledDiffuse;

//function declerations
vec3 directionalLight();

void main()
{
    fragNormal = texture(normalMap, texCoords).rgb;
    fragNormal = normalize(fragNormal * 2.f - 1.f);
    fragNormal = mat3(model_normal) * fragNormal;

    fragColor = vec4(directionalLight(), 1.f);
}

//function definitions
vec3 directionalLight()
{
    sampledDiffuse = texture(diffuseMap, texCoords);
    vec3 lightDir = normalize(light.direction);

    vec3 ambient = light.ambient;

    float diffuse_value = max(dot(fragNormal, lightDir), 0);
    vec3 diffuse = light.diffuse * diffuse_value;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfway = normalize(lightDir + viewDir);
    float spec = pow(max(dot(fragNormal, halfway), 0.f), material.shininess);
    vec3 specular = light.specular * spec;

    return (ambient + diffuse + specular) * vec3(sampledDiffuse);
}
