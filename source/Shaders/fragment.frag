#version 330 core

in vec2 TextureCoords;

out vec4 FragmentColor;

uniform float texturesMixCoefficient;

uniform sampler2D backTexture;
uniform sampler2D frontTexture;

void main()
{
  FragmentColor = mix(texture(backTexture, TextureCoords).rgba,
    texture(frontTexture, TextureCoords),
    vec4(texture(frontTexture, TextureCoords)).a * texturesMixCoefficient);
}
