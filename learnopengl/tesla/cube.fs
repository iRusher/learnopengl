#version 330 core
out vec4 FragColor;

in vec4 fColor;
in vec2 Pos;

void main()
{
    FragColor = fColor;
}