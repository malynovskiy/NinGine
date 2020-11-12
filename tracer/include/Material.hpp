#pragma once
#include "Vector.hpp"

namespace ningine
{
struct Material
{
  Material()
    : diffuse_color(), albedo(1.0f, 0.0f, 0.0f, 0.0f), specular_exponent(), refractive_index(1.0f)
  {
  }

  Material(const math::Vec3f &Color,
    const math::Vec4f &Albedo,
    const float &Specular,
    const float &Refractive_index)
    : diffuse_color(Color), albedo(Albedo), specular_exponent(Specular),
      refractive_index(Refractive_index)
  {
  }

  math::Vec3f diffuse_color;
  math::Vec4f albedo;
  float specular_exponent;
  float refractive_index;
};


// few Material presets
namespace materials
{
  constexpr Material ivory(Vec3f(0.4f, 0.4f, 0.3f), Vec4f(0.6f, 0.3f, 0.1f, 0.0f), 50.0f, 1.0f);
  constexpr Material glass(Vec3f(0.6f, 0.7f, 0.8f), Vec4f(0.0f, 0.5f, 0.1f, 0.8f), 125.0f, 1.5f);
  constexpr Material red_rubber(Vec3f(0.3f, 0.1f, 0.1f), Vec4f(0.9f, 0.1f, 0.0f, 0.0f), 10.0f, 1.0f);
  constexpr Material mirror(Vec3f(1.0f, 1.0f, 1.0f), Vec4f(0.0f, 10.0f, 0.8f, 0.0f), 1425.0f, 1.0f);
}

}// namespace ningine
