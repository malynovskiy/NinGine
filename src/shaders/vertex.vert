#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTextureCoords;

out vec3 Color;
out vec2 TextureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection* view * model * vec4(inPos, 1.0);

  //Color = inColor;
  TextureCoords = vec2(inTextureCoords.x, inTextureCoords.y);
}
