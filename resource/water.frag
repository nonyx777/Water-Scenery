#version 330 core

//struct Material{
//    sampler2D texture_diffuse1;
//    sampler2D texture_specular1;
//    float shininess;
//};
//
//struct Light{
//    vec3 direction;
//    vec3 position;
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//
//    //attenuation
//    float kc;
//    float kl;
//    float kq;
//};
//
//uniform Material material;
//uniform Light light;

out vec4 fragColor;
in vec2 texCoords;
in vec3 fragPos;
//uniform sampler2D texture_diffuse1;
//uniform vec3 view_pos;

//phone lighting model variables
//vec3 ambient;
//vec3 diffuse;
//vec3 specular;

//function declaration
//float pointLight();
//vec3 lightModelCalc();

//vec4 sampled_diffuse = texture(material.texture_diffuse1, texCoords);
//vec4 sampled_specular = texture(material.texture_specular1, texCoords);

void main()
{
    //fragColor = vec4(lightModelCalc() * pointLight(), 1.f);
    fragColor = vec4(0.f, 0.f, 1.f, 1.f);
}

//function definition
//float pointLight()
//{
//    float distance = length(light.position - fragPos);
//    float attenuation = light.kc + light.kl * distance + light.kq * distance * distance;
//    float luminosity = 1.f/attenuation;
//
//    return luminosity;
//}

//vec3 lightModelCalc()
//{
//    //common
//    vec3 light_dir = normalize(light.position - fragPos);
//
//    ambient = light.ambient * vec3(sampled_diffuse);
//
//    float diffuse_value = max(dot(fragNormal, light_dir), 0.f);
//    diffuse = diffuse_value * light.diffuse * vec3(sampled_diffuse);
//
//    vec3 view_dir = normalize(view_pos - fragPos);
//    vec3 reflected = reflect(-light_dir, fragNormal);
//    float specular_intensity = pow(max(dot(view_dir, reflected), 0.f), material.shininess);
//    specular = specular_intensity * light.specular * vec3(sampled_specular);
//
//    vec3 result = ambient + diffuse + specular;
//
//    return result;
//}