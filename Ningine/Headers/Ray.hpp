#pragma once
#include "Vector.hpp"
namespace Ningine
{
struct Ray
{
  math::Vec3f origin;
  math::Vec3f direction;
  float tmin;
  float tmax;
};
}// namespace Ningine

