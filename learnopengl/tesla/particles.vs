#version 330 core

layout (location = 0) in vec3 pVertices;
layout (location = 1) in vec4 pPosition;
layout (location = 2) in vec4 pColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	float particleSize = pPosition.w;
	vec3 particleCenter_wordspace = pPosition.xyz;

	vec3 vertexPosition_worldspace = pVertices +  particleCenter_wordspace;

    gl_Position =  projection * view * model * vec4(pVertices,1.0);

}