#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTextCoord;

out vec2 TextCoord;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
	TextCoord = aTextCoord;
}