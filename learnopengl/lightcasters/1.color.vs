#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTextCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Norm;
out vec3 FragPos;
out vec2 TextCoord;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(view * vec4(aPos, 1.0));
	Norm = mat3(transpose(inverse(model))) * aNorm;
    TextCoord = aTextCoord;
}