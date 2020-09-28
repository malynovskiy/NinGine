#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTextureCoords;

out vec3 Color;
out vec2 TextureCoords;

uniform mat4 transform;

void main()
{
  gl_Position = transform * vec4(inPos, 1.0);

  //Color = inColor;
  TextureCoords = vec2(inTextureCoords.x, inTextureCoords.y);
}
