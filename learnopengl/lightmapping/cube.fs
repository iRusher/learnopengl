#version 330 core
out vec4 FragColor;

in vec3 Norm;
in vec3 FragPos;
in vec2 TextCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

// uniform float ambientStrength;
// uniform float specularStrength;

struct Material {
//     vec3 ambient;
//     sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
//     vec3 specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;


void main()
{
//     float ambientStrength = 0.1;
    vec3 ambient = light.ambient * vec3(texture(material.diffuse,TextCoord));

// diff
    vec3 norm = normalize(Norm);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm ,lightDir) , 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TextCoord));

    // specular
//     float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    // reflect是入射光线，lightDir指向光源，取反即是入射光方向
    vec3 reflectDir = reflect(-lightDir,norm);

    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = light.specular *  spec * vec3(texture(material.specular,TextCoord));

    vec3 result = ( ambient + diffuse + specular ) * objectColor;
    FragColor = vec4(result, 1.0);
}
