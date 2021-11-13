#version 330 core
out vec4 FragColor;

in vec3 Norm;
in vec3 FragPos;
in vec2 TextCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutoff;
    float outerCutOff;

};

uniform Material material;
uniform Light light;


void main()
{

    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir,normalize(-light.direction));
    float epsilon = light.cutoff - light.outerCutOff;
    float intensity = clamp( (theta - light.outerCutOff)/epsilon ,0.0,1.0);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse,TextCoord));

    // diff
    vec3 norm = normalize(Norm);
    float diff = max(dot(norm ,lightDir) , 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TextCoord));

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    // reflect需要光线入射方向，lightDir指向光源，取反即是入射光方向
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = light.specular *  spec * vec3(texture(material.specular,TextCoord));

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance );

    ambient *= intensity * attenuation;
    diffuse *= intensity * attenuation;
    specular *= intensity * attenuation;

    vec3 result = ( ambient + diffuse + specular );
    FragColor = vec4(result, 1.0);

}
