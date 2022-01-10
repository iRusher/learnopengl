#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aDelta;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int pass;

out vec3 outPos; //transform feedback

uniform sampler1D t;

void main()
{
    if (pass == 0) {
        float r = texelFetch(t,gl_VertexID,0).r;
        outPos = vec3(aPos.x,aPos.y + aDelta + r, 0.0);
    } else {
        gl_PointSize = gl_InstanceID * 1;
        vec4 pos = vec4(aPos,1.0);
        gl_Position = pos;
    }
}