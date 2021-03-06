#version 330 core
in vec2 TexCoords;
in vec4 ParticleColor;

out vec4 FragColor;

uniform sampler2D sprite;

void main()
{
    FragColor = (texture(sprite, TexCoords) * ParticleColor);
//     FragColor = (texture(sprite, TexCoords) * vec4(1.0,0,0,1.0));
}