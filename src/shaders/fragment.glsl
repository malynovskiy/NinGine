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
  FragmentColor = mix(texture(woodTexture, TextureCoords),
                    texture(memeTexture, TextureCoords),
                    texturesMixCoefficient)
                  * vec4(Color, 1.0);
}
