#pragma once
#include "Vector.hpp"

namespace ningine
{
struct LightSource
{
  LightSource(const math::Vec3f &Position, const float &Intensity)
    : position(Position), intensity(Intensity)
  {
  }

  math::Vec3f position;
  float intensity;
};
}
