#version 330 core
out vec4 FragColor;

uniform sampler2D container;

in vec2 Pos;

void main()
{
    float f = texelFetch(container,vec2(0,0),0).r;
//     float f = 1.0f;
    FragColor = vec4(f,1.0,1.0,1.0);
}