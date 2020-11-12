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
  const Material ivory(
    math::Vec3f(0.4f, 0.4f, 0.3f), math::Vec4f(0.6f, 0.3f, 0.1f, 0.0f), 50.0f, 1.0f);
  const Material glass(
    math::Vec3f(0.6f, 0.7f, 0.8f), math::Vec4f(0.0f, 0.5f, 0.1f, 0.8f), 125.0f, 1.5f);
  const Material red_rubber(
    math::Vec3f(0.3f, 0.1f, 0.1f), math::Vec4f(0.9f, 0.1f, 0.0f, 0.0f), 10.0f, 1.0f);
  const Material mirror(
    math::Vec3f(1.0f, 1.0f, 1.0f), math::Vec4f(0.0f, 10.0f, 0.8f, 0.0f), 1425.0f, 1.0f);
}// namespace materials
}// namespace ningine
