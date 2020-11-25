#define SPHERE_DATA_SIZE 13
#define LIGHT_DATA_SIZE 4
#define BACKGROUND_COLOR (float3)(0.2, 0.7, 0.87)
#define epsilon 1e-3f

#define get_index_s(i, offset) ((i * SPHERE_DATA_SIZE) + offset)
#define get_index_l(i, offset) ((i * LIGHT_DATA_SIZE) + offset)

#define light_pos_x_i(i) (get_index_l(i, 0))
#define light_pos_y_i(i) (get_index_l(i, 1))
#define light_pos_z_i(i) (get_index_l(i, 2))

#define sphere_pos_x_i(i) (get_index_s(i, 0))
#define sphere_pos_y_i(i) (get_index_s(i, 1))
#define sphere_pos_z_i(i) (get_index_s(i, 2))

#define sphere_radius_i(i) (get_index_s(i, 3))

#define sphere_color_x_i(i) (get_index_s(i, 4))
#define sphere_color_y_i(i) (get_index_s(i, 5))
#define sphere_color_z_i(i) (get_index_s(i, 6))

#define sphere_albedo_x_i(i) (get_index_s(i, 7))
#define sphere_albedo_y_i(i) (get_index_s(i, 8))
#define sphere_albedo_z_i(i) (get_index_s(i, 9))
#define sphere_albedo_w_i(i) (get_index_s(i, 10))

#define sphere_specular_i(i) (get_index_s(i, 11))
#define sphere_refractive_i(i) (get_index_s(i, 12))

#define sphere_pos_vec3(i)                  \
  ((float3)(spheresData[sphere_pos_x_i(i)], \
    spheresData[sphere_pos_y_i(i)],         \
    spheresData[sphere_pos_z_i(i)]))

#define sphere_radius(i) (spheresData[sphere_radius_i(i)])

#define sphere_color_vec3(i)                  \
  ((float3)(spheresData[sphere_color_x_i(i)], \
    spheresData[sphere_color_y_i(i)],         \
    spheresData[sphere_color_z_i(i)]))

#define sphere_albedo_vec4(i)                  \
  ((float4)(spheresData[sphere_albedo_x_i(i)], \
    spheresData[sphere_albedo_y_i(i)],         \
    spheresData[sphere_albedo_z_i(i)],         \
    spheresData[sphere_albedo_w_i(i)]))

#define sphere_specular(i) (spheresData[sphere_specular_i(i)])
#define sphere_refractive(i) (spheresData[sphere_refractive_i(i)])

#define light_pos_vec3(i)                   \
  ((float3)(lightingData[light_pos_x_i(i)], \
    lightingData[light_pos_y_i(i)],         \
    lightingData[light_pos_z_i(i)]))

struct Material
{
  float3 diffuse_color;
  float4 albedo;
  float specular_exponent;
  float refractive_index;
};

typedef struct Material Material;

void material_init(Material *material)
{
  material->diffuse_color = 0;
  material->albedo = (float4)(1.0f, 0.0f, 0.0f, 0.0f);
  material->specular_exponent = 0;
  material->refractive_index = 1.0f;
}

void place_material(Material *material, __global float *spheresData, int index)
{
  material->diffuse_color = sphere_color_vec3(index);
  material->albedo = sphere_albedo_vec4(index);
  material->specular_exponent = sphere_specular(index);
  // material->refractive_index = sphere_refractive(index);
  material->refractive_index = spheresData[(index * SPHERE_DATA_SIZE) + 12];
}

float3 refract(const float3 *direction, const float3 *normal, float *refractive_index)
{
  // cosine of insidence angle
  float cosi = -max(-1.0f, min(1.0f, dot(*direction, *normal)));
  float etai = 1;
  float etat = *refractive_index;
  float3 n = *normal;
  if (cosi < 0)
  {
    cosi = -cosi;

    // swap
    etat = etai;
    etai = *refractive_index;

    n = -n;
  }
  // probably should use fraction here
  float eta = etai / etat;
  float k = 1.0f - eta * eta * (1.0f - cosi * cosi);

  if (k < 0)
    return (float3)(1, 0, 0);
  else
    return *direction * eta + n * (eta * cosi - sqrt(k));
}

// possible optimization here:
//    pass already created vector by pointer,
//    and only calculate here
float3 reflect(const float3 *direction, const float3 *normal)
{
  float3 res = *direction - *normal * 2.0f * dot(*direction, *normal);
  return res;
}

bool sphere_ray_intersect(float3 *origin,
  float3 *direction,
  float3 *spherePos,
  float sphereRadius,
  float *intersectionDistance)
{
  // vector from ray's origin to the center of sphere
  float3 L = *spherePos - *origin;
  
  const float projectionLength = dot(L, *direction);
  const float d2 = dot(L, L) - projectionLength * projectionLength;

  const float r2 = sphereRadius * sphereRadius;
  // whether the ray and sphere intersect
  if (d2 > r2)
    return false;

  float thc = sqrt(r2 - d2);
  // distance to the first point on the sphere
  *intersectionDistance = projectionLength - thc;

  // distance to the second point on the sphere
  float t1 = projectionLength + thc;

  if (*intersectionDistance < 0)
    *intersectionDistance = t1;

  if (*intersectionDistance < 0)
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

    float3 spherePos = sphere_pos_vec3(i);

    if (sphere_ray_intersect(origin, direction, &spherePos, spheresData[sphere_radius(i)], &dist_i)
        && dist_i < spheres_dist)
    {
      spheres_dist = dist_i;
      *hit = *origin + *direction * dist_i;
      *normal = normalize(*hit - spherePos);

      // workaround for now, probably should set some index here and use it
      // later -> idea is to avoid coppying beforehand
      place_material(material, spheresData, i);
    }
  }

  return spheres_dist;
  // CHECKBOARD DROPPED FOR NOW

  // float checkerboard_dist = FLT_MAX;
  // if (fabs(direction.y) > 1e-3)
  // {
  //   float d = -(origin.y + 4) / direction.y;
  //   math::Vec3f pt = origin + direction * d;
  //   if (d > 0 && fabs(pt.x) < 10 && pt.z < -10 && pt.z > -30 && d < spheres_dist)
  //   {
  //     checkerboard_dist = d;
  //     hit = pt;
  //     normal = math::Vec3f(0.0f, 1.0f, 0.0f);
  //     material.diffuse_color = (int(.5 * hit.x + 1000) + int(.5 * hit.z)) & 1
  //                                ? math::Vec3f(1, 1, 1)
  //                                : math::Vec3f(1, .7, .3);
  //     material.diffuse_color = material.diffuse_color * .3;
  //   }
  // }

  // return std::min(spheres_dist, checkerboard_dist) < 1000;
}

float4 cast_ray(float3 *origin,
  float3 *direction,
  __global float *spheresData,
  int numSpheres,
  __global float *lighting,
  int numLightSources,
  int depth)
{
  // intersection point and normal in that point
  float3 point, normal;
  Material material;

  if (/*depth > 2
      || */!scene_intersect(origin, direction, spheresData, numSpheres, &point, &normal, &material))
    return BACKGROUND_COLOR;
  return (float4)(material.diffuse_color, 1);

  // float3 reflect_dir = reflect(direction, normal).normalize();
  // float3 refract_dir = refract(direction, normal, material.refractive_index).normalize();

  // Vec3f reflect_origin = reflect_dir * normal < 0 ? point - normal * 1e-3 : point + normal * 1e-3;
  // Vec3f refract_origin = refract_dir * normal < 0 ? point - normal * 1e-3 : point + normal * 1e-3;

  // Vec3f reflect_color = cast_ray(reflect_origin, reflect_dir, spheres, lightSources, depth + 1);
  // Vec3f refract_color = cast_ray(refract_origin, refract_dir, spheres, lightSources, depth + 1);

  // float diffuse_light_intensity{}, specular_light_intensity{};
  // for (const auto &lightSrc : lightSources)
  // {
  //   Vec3f ld = lightSrc.position - point;
  //   float light_distance = ld.norm();
  //   Vec3f light_dir = ld.normalize();

  //   Vec3f shadow_orig = light_dir * normal < 0 ? point - normal * 1e-3 : point + normal * 1e-3;
  //   Vec3f shadow_pt, shadow_n;
  //   Material tmpMaterial;

  //   if (scene_intersect(shadow_orig, light_dir, spheres, shadow_pt, shadow_n, tmpMaterial)
  //       && (shadow_pt - shadow_orig).norm() < light_distance)
  //     continue;

  //   diffuse_light_intensity += lightSrc.intensity * std::max(0.0f, light_dir * normal);
  //   specular_light_intensity +=
  //     powf(std::max(0.0f, reflect(light_dir, normal) * direction), material.specular_exponent)
  //     * lightSrc.intensity;
  // }
  // return material.diffuse_color * diffuse_light_intensity * material.albedo[0]
  //        + Vec3f(1.0f, 1.0f, 1.0f) * specular_light_intensity * material.albedo[1]
  //        + reflect_color * material.albedo[2] + refract_color * material.albedo[3];
}

__kernel void raytracer(__write_only image2d_t image,
  float3 cameraPos,
  float screenDistance,
  __global float *spheresData,
  int numSpheres,
  __global float *lighting,
  int numLightSources)
{
  int2 pos = (int2)(get_global_id(0), get_global_id(1));

  float3 dir = (float3)(pos.x, pos.y, screenDistance);
  dir -= cameraPos;
  dir = normalize(dir);

  float4 color = cast_ray(&cameraPos, &dir, spheresData, numSpheres, lighting, numLightSources, 0);

  write_imagef(image, pos, color);
}

// float4 calculatePixelColor(float3 *cameraPos,
//   float screenDistance,
//   int2 *pixelPos,
//   __global float *spheresData,
//   int numSpheres,
//   __global float *lighting)
// {
//   float3 dir = (float3)(pixelPos->x, pixelPos->y, screenDistance);

//   dir -= *cameraPos;
//   dir = normalize(dir);

//   return cast_ray(cameraPos, &dir, spheresData, numSpheres, lighting, 0);
// }
