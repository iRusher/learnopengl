#version 330 core
in vec2 TextCoord;

out vec4 FragColor;

uniform sampler2D texture0;

void main()
{
    FragColor = vec4(texture(texture0,TextCoord),1.0);

//     texture(texture0,TextCoord)
}
