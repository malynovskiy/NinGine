#version 150

in vec2 ex_TexCoord;
in vec3 ex_Color;

out vec4 out_Color;

uniform sampler2D tex;

void main(void)
{
	out_Color = vec4(ex_Color, 1.0) * texture(tex, ex_TexCoord);
}