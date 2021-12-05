#version 330 core

in VS_OUT {
    vec2 TextCoord;
} fs_in;

out vec4 FragColor;

uniform sampler2D texture0;

void main()
{
    FragColor = vec4(1.0,1.0,0.0,1.0);
}
