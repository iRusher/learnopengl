#version 330 core
out vec4 FragColor;

in vec4 fColor;
in vec2 Pos;

void main()
{
//     FragColor = vec4(1.0,0.0,0.0,0.4);
//     FragColor = fColor;
    FragColor = vec4(fColor.xyz,0.3);
}