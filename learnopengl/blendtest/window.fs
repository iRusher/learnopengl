#version 330 core
out vec4 FragColor;
in vec2 TextCoord;

uniform sampler2D texture0;

void main()
{
    FragColor = texture(texture0,TextCoord);
//     FragColor = vec4(1.0,0.0,0.0,1.0);
}
