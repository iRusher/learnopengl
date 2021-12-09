#version 330 core
out vec4 FragColor;

in vec4 fColor;
in vec2 Pos;

uniform float iTime;

#define PI 3.1415926

void main()
{
    FragColor = vec4(fColor.xyz,0.3);
//          vec2 iResolution = vec2(800,600);
//     	    vec2 uv = gl_FragCoord.xy / iResolution.xy;
//          vec2 pos = PI *(uv*2.-1.);
//          vec4 color =vec4(0.2, 0.6, 1., 1.)* abs(sin(20.*pos.y + 20.*sin(pos.x + iTime)));
//          FragColor = color;

}