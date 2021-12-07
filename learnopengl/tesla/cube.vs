#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;

out vec4 fColor;
out vec2 Pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float iTime;


void main()
{
    // vec4 pos = vec4(aPos.x,aPos.y,sin(iTime + aPos.y),1.0);
    vec4 pos = vec4(aPos.xy,0.0,1.0);
    gl_Position =  projection * view * model * pos;
    fColor = aColor;
    Pos = aPos;
}