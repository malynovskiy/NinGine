#version 330 core

in vec3 Color;
in vec2 TextureCoords;

out vec4 FragmentColor;

uniform vec3 colorBlending;

uniform float texturesMixCoefficient;

uniform sampler2D backTexture;
uniform sampler2D frontTexture;

void main()
{
  FragmentColor = mix(texture(backTexture, TextureCoords).rgba,
    texture(frontTexture, TextureCoords) * vec4(colorBlending, 1.0),
    vec4(texture(frontTexture, TextureCoords)).a * texturesMixCoefficient);
}
