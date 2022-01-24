#version 330 core

in vec2 TextCoord;

out vec4 FragColor;

uniform vec3 objectColor;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
//     FragColor = texture(texture1, TextCoord);
    FragColor = mix(texture(texture1, TextCoord),texture(texture2, TextCoord), 0.2);
}