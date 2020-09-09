#version 330 core

uniform vec3 colorBlending;
in vec3 Color;
out vec4 FragmentColor;

void main()
{
    FragmentColor = vec4(colorBlending * Color, 1.0);
}
