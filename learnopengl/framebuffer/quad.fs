#version 330 core
in vec2 TextCoord;

out vec4 FragColor;

uniform sampler2D texture0;

void main()
{
    FragColor = texture(texture0,TextCoord);

//     texture(texture0,TextCoord)
}
