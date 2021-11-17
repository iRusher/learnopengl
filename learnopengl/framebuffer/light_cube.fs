#version 330 core
out vec4 FragColor;

uniform vec3 LightCubeColor;

void main()
{
    FragColor = vec4(LightCubeColor,1.0); // set alle 4 vector values to 1.0
}