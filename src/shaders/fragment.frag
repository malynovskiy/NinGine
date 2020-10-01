#version 330 core

in vec3 Color;
in vec2 TextureCoords;

out vec4 FragmentColor;

uniform vec3 colorBlending;

uniform float texturesMixCoefficient;

uniform sampler2D woodTexture;
uniform sampler2D memeTexture;

void main()
{
  FragmentColor = mix(texture(woodTexture, TextureCoords).rgba,
    texture(memeTexture, TextureCoords) * vec4(colorBlending, 1.0),
    vec4(texture(memeTexture, TextureCoords)).a * texturesMixCoefficient);
}
