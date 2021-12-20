#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aDelta;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_PointSize = gl_InstanceID * 2;
    vec4 pos = vec4(aPos.x  + aDelta,aPos.y + aDelta, 0.0, 1.0);
    gl_Position = pos;
}