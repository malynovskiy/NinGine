#pragma once
#include "Vector.hpp"
#include "Material.hpp"

namespace ningine
{
struct Sphere
{
  math::Vec3f center;
  float radius;
  Material material;

  Sphere(const math::Vec3f &Center, const float &Radius, const Material &Material)
    : center(Center), radius(Radius), material(Material)
  {
  }

  bool ray_intersect(const math::Vec3f &ray_origin,
    const math::Vec3f &ray_direction,
    float &intersectionDistance) const
  {
    // vector from ray's origin to the center of sphere
    const math::Vec3f L = center - ray_origin;

    const float projectionLength = L * ray_direction;
    const float d2 = L * L - projectionLength * projectionLength;

    const float r2 = radius * radius;
    // whether the ray and sphere intersect
    if (d2 > r2)
      return false;

    float thc = sqrtf(r2 - d2);
    // distance to the first point on the sphere
    intersectionDistance = projectionLength - thc;

    // distance to the second point on the sphere
    float t1 = projectionLength + thc;

    if (intersectionDistance < 0)
      intersectionDistance = t1;

    if (intersectionDistance < 0)
      return false;

    return true;
  }
};
}
