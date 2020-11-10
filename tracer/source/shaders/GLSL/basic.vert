#version 150

in  vec3 in_Position;
in  vec3 in_Color; 
in  vec2 in_TexCoord;

out vec3 ex_Color;
out vec2 ex_TexCoord;

void main(void)
{
	gl_Position = vec4(in_Position, 1.0);

	ex_TexCoord = in_TexCoord;

	ex_Color = in_Color;
}