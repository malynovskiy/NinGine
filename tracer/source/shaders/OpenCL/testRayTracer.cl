#define SPHERE_DATA_SIZE 20

//#define normalize(x) ({ x / norm(x); })

inline float norm(float3 &vec)
{
  return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}

struct Material
{
  float3 diffuse_color;
  float4 albedo;
  float specular_exponent;
  float refractive_index;
};

typedef struct Material Material;

bool sphere_ray_intersect(float3 *origin,
  float3 *direction,
  float3 *spherePos,
  float sphereRadius,
  float *intersectionDistance)
{
  // vector from ray's origin to the center of sphere
  float3 L = *spherePos - *origin;

  const float projectionLength = L * direction;
  const float d2 = L * L - projectionLength * projectionLength;

  const float r2 = sphereRadius * sphereRadius;
  // whether the ray and sphere intersect
  if (d2 > r2)
    return false;

  float thc = sqrt(r2 - d2);
  // distance to the first point on the sphere
  *intersectionDistance = projectionLength - thc;

  // distance to the second point on the sphere
  float t1 = projectionLength + thc;

  if (intersectionDistance < 0)
    intersectionDistance = t1;

  if (intersectionDistance < 0)
    return false;

  return true;
}

bool scene_intersect(float3 *origin,
  float3 *direction,
  __global float *spheresData,
  int numSpheres,
  float3 *hit,
  float3 *normal,
  Material *material)
{
  float spheres_dist = FLT_MAX;

  for (int i = 0; i < numSpheres; i++)
  {
    float dist_i;

    float3 spherePos = (float3)(sphereData[(i * SPHERE_DATA_SIZE) + 0],
      sphereData[(i * SPHERE_DATA_SIZE) + 1],
      sphereData[(i * SPHERE_DATA_SIZE) + 2]);

    if (sphere_ray_intersect(
          origin, direction, &spherePos, sphereData[(i * SPHERE_DATA_SIZE) + 3], &dist_i)
        && dist_i < spheres_dist)
    {
      spheres_dist = dist_i;
      *hit = *origin + *direction * dist_i;
      *normal = normalize(*hit - spherePos);
      *material = sphere.material;
    }
  }
  float checkerboard_dist = FLT_MAX;
  if (fabs(direction.y) > 1e-3)
  {
    float d = -(origin.y + 4) / direction.y;
    math::Vec3f pt = origin + direction * d;
    if (d > 0 && fabs(pt.x) < 10 && pt.z < -10 && pt.z > -30 && d < spheres_dist)
    {
      checkerboard_dist = d;
      hit = pt;
      normal = math::Vec3f(0.0f, 1.0f, 0.0f);
      material.diffuse_color = (int(.5 * hit.x + 1000) + int(.5 * hit.z)) & 1
                                 ? math::Vec3f(1, 1, 1)
                                 : math::Vec3f(1, .7, .3);
      material.diffuse_color = material.diffuse_color * .3;
    }
  }

  return std::min(spheres_dist, checkerboard_dist) < 1000;
}

cast_ray(float3 *origin,
  float3 *direction,
  __global float *spheresData,
  int numSpheres,
  __global float *lighting int depth)
{
  // intersection point and normal in that point
  float3 point, normal;
  Material material;

  if (depth > 4
      || !scene_intersect(origin, direction, spheresData, numSpheres, &point, &normal, &material))
    return backgroundColor;

  Vec3f reflect_dir = reflect(direction, normal).normalize();
  Vec3f refract_dir = refract(direction, normal, material.refractive_index).normalize();

  Vec3f reflect_origin = reflect_dir * normal < 0 ? point - normal * 1e-3 : point + normal * 1e-3;
  Vec3f refract_origin = refract_dir * normal < 0 ? point - normal * 1e-3 : point + normal * 1e-3;

  Vec3f reflect_color = cast_ray(reflect_origin, reflect_dir, spheres, lightSources, depth + 1);
  Vec3f refract_color = cast_ray(refract_origin, refract_dir, spheres, lightSources, depth + 1);

  float diffuse_light_intensity{}, specular_light_intensity{};
  for (const auto &lightSrc : lightSources)
  {
    Vec3f ld = lightSrc.position - point;
    float light_distance = ld.norm();
    Vec3f light_dir = ld.normalize();

    Vec3f shadow_orig = light_dir * normal < 0 ? point - normal * 1e-3 : point + normal * 1e-3;
    Vec3f shadow_pt, shadow_n;
    Material tmpMaterial;

    if (scene_intersect(shadow_orig, light_dir, spheres, shadow_pt, shadow_n, tmpMaterial)
        && (shadow_pt - shadow_orig).norm() < light_distance)
      continue;

    diffuse_light_intensity += lightSrc.intensity * std::max(0.0f, light_dir * normal);
    specular_light_intensity +=
      powf(std::max(0.0f, reflect(light_dir, normal) * direction), material.specular_exponent)
      * lightSrc.intensity;
  }
  return material.diffuse_color * diffuse_light_intensity * material.albedo[0]
         + Vec3f(1.0f, 1.0f, 1.0f) * specular_light_intensity * material.albedo[1]
         + reflect_color * material.albedo[2] + refract_color * material.albedo[3];
}


float4 calculatePixelColor(float3 *cameraPos,
  float screenDistance,
  int2 *pixelPos,
  __global float *spheresData,
  int numSpheres,
  __global float *lighting)
{
  float3 dir = (float3)(pixelPos->x, pixelPos->y, screenDistance);

  dir -= *cameraPos;
  dir = normalize(dir);

  return cast_ray(cameraPos, &dir, spheresData, numSpheres, lighting, 0);
}


__kernel void raytracer(__write_only image2d_t image,
  float3 cameraPos,
  float screenDistance,
  __global float *spheresData,
  int numSpheres,
  __global float *lighting)
{
  int2 pos = (int2)(get_global_id(0), get_global_id(1));

  float4 color =
    calculatePixelColor(&cameraPos, screenDistance, &pos, spheresData, numSpheres, lighting);
  write_imagef(image, pos, color);
}