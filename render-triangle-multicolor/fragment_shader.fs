#version 330 core

out vec4 FragColor;

in vec4 outColor;

void main()
{
    FragColor = vec4(outColor, 1.0);
}