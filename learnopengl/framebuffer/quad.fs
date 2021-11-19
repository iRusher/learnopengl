#version 330 core
in vec2 TextCoord;

out vec4 FragColor;

uniform sampler2D texture0;


float near = 0.1;
float far  = 100.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
//     FragColor = texture(texture0,TextCoord); // 原始的深度贴图
//     FragColor = vec4(vec3(texture(texture0,TextCoord),1.0); // 转换为黑白的
    FragColor = vec4(vec3(LinearizeDepth(texture(texture0,TextCoord).r) / far),1.0); // 将屏幕空间中非线性的深度值变换至线性深度值

}
