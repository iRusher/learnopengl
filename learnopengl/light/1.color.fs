#version 330 core
out vec4 FragColor;
in vec3 Norm;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = lightColor * ambientStrength;

    vec3 norm = normalize(Norm);
    vec3 lightDir = normalize(lightPos - FragPos);

    // diff
    float diff = max(dot(norm ,lightDir) , 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    // reflect是入射光线，lightDir指向光源，取反即是入射光方向
    vec3 reflectDir = reflect(-lightDir,norm);


    float spec = pow(max(dot(viewDir,reflectDir),0.0),64);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = ( ambient + diffuse + specular )* objectColor;
    FragColor = vec4(result, 1.0);
}