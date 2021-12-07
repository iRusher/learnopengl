#version 330 core
out vec4 FragColor;

in vec3 fColor;
in vec2 Pos;

void main()
{

    float r = 1 - Pos.x;

    FragColor = vec4(r,0.0,0.0, 1.0);
}