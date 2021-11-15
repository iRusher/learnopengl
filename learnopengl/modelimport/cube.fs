#version 330 core
out vec4 FragColor;

in vec3 Norm;
in vec3 FragPos;
in vec2 TextCoord;

uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform SpotLight spotLight;


// 计算定向光
vec3 CalDirectLight(DirLight dirLight,vec3 normal,vec3 viewDir);
// 计算点光源
vec3 CalPoint(PointLight light,vec3 normal,vec3 viewDir);
// 聚光
vec3 CalSpotLight(SpotLight light,vec3 normal,vec3 viewDir);

vec4 TestCalSpot(SpotLight light,vec3 normal, vec3 viewDir);
void main()
{
    vec3 normal = normalize(Norm);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = CalDirectLight(dirLight,normal,viewDir);
    for(int i = 0; i < NR_POINT_LIGHTS;i++)
        result += CalPoint(pointLights[i],normal,viewDir);
    result += CalSpotLight(spotLight,normal,viewDir);
    FragColor = vec4(result,1.0);
}

vec3 CalDirectLight(DirLight light,vec3 normal,vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 漫反射
    float diff = max(dot(lightDir,normal), 0.0);
    // 镜面光反射
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);

    vec3 ambient    = light.ambient  * vec3(texture(material.diffuse,TextCoord));
    vec3 diffuse    = light.diffuse  * diff * vec3(texture(material.diffuse, TextCoord));
    vec3 specular   = light.specular * spec * vec3(texture(material.specular,TextCoord));

    return (ambient + diffuse + specular);
}

vec3 CalPoint(PointLight light,vec3 normal,vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);
    // 漫反射
    float diff = max(dot(lightDir,normal), 0.0);
    // 镜面光反射
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance );

    vec3 ambient    = light.ambient  * vec3(texture(material.diffuse,TextCoord));
    vec3 diffuse    = light.diffuse  * diff * vec3(texture(material.diffuse, TextCoord));
    vec3 specular   = light.specular * spec * vec3(texture(material.specular,TextCoord));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalSpotLight(SpotLight light,vec3 normal,vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir,normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp( (theta - light.outerCutOff)/epsilon ,0.0,1.0);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse,TextCoord));

    // diff
    float diff = max(dot(normal ,lightDir) , 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TextCoord));

    // specular
    // reflect需要光线入射方向，lightDir指向光源，取反即是入射光方向
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = light.specular *  spec * vec3(texture(material.specular,TextCoord));

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance );

    ambient *= intensity * attenuation;
    diffuse *= intensity * attenuation;
    specular *= intensity * attenuation;

    return ( ambient + diffuse + specular );
}

vec4 TestCalSpot(SpotLight light,vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir,normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp( (theta - light.outerCutOff)/epsilon ,0.0,1.0);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse,TextCoord));

    // diff
//     vec3 norm = normalize(Norm);
    float diff = max(dot(normal ,lightDir) , 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TextCoord));

    // specular
//     vec3 viewDir = normalize(viewPos - FragPos);
    // reflect需要光线入射方向，lightDir指向光源，取反即是入射光方向
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = light.specular *  spec * vec3(texture(material.specular,TextCoord));

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance );

    ambient *= intensity * attenuation;
    diffuse *= intensity * attenuation;
    specular *= intensity * attenuation;

    vec3 result = ( ambient + diffuse + specular );
    FragColor = vec4(result, 1.0);

    return FragColor;

}