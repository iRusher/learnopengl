#version 330 core
layout (location = 1) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 TextCoords;

void main()
{
	gl_Position = projection * view * vec4(aPos, 1.0);
	TextCoords = aPos;
}