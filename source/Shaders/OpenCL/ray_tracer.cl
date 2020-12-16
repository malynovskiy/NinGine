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

struct Ray
{
  float3 origin;
  float3 direction
};

typedef struct Ray Ray;

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
  material->refractive_index = sphere_refractive(index);
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

bool sphere_ray_intersect(
  float3 *origin, float3 *direction, float3 *spherePos, float radius, float *dist)
{
  // vector from ray's origin to the center of sphere
  float3 L = *spherePos - *origin;

  const float projLenght = dot(L, *direction);
  const float d2 = dot(L, L) - projLenght * projLenght;

  const float r2 = radius * radius;
  // whether the ray and sphere intersect
  if (d2 > r2)
    return false;

  float thc = sqrt(r2 - d2);
  // distance to the first point on the sphere
  *dist = projLenght - thc;

  // distance to the second point on the sphere
  float t1 = projLenght + thc;

  if (*dist < 0)
    *dist = t1;

  if (*dist < 0)
    return false;

  return true;
}

float triangle_ray_intersect(const float3 *orig,
  const float3 *dir,
  const float3 *v0,
  const float3 *v1,
  const float3 *v2,
  float3 *n,
  float *t)
{
  float3 e1 = *v1 - *v0;
  float3 e2 = *v2 - *v0;

  float3 pvec = cross(*dir, e2);
  float det = dot(e1, pvec);

  if (det < epsilon && det > -epsilon)
    return false;

  const float inv_det = 1.0f / det;
  float3 tvec = *orig - *v0;
  float u = dot(tvec, pvec) * inv_det;
  if (u < 0 || u > 1)
    return false;

  float3 qvec = cross(tvec, e1);
  float v = dot(*dir, qvec) * inv_det;
  if (v < 0 || u + v > 1)
    return false;

  const float dist = dot(e2, qvec) * inv_det;
  if (dist < 0)
    return false;

  *n = normalize(cross(e1, e2));
  *t = dist;
  return true;
}

bool plane_ray_intersect(
  const float3 *orig, const float3 *dir, const float3 p0, const float3 *N, float *dist)
{
  const float denom = dot(*N, *dir);
  if (fabs(denom) < 1e-3f)
    return false;

  float3 p0orig = p0 - *orig;
  float t = dot(p0orig, *N) / denom;
  if (t >= 0)
  {
    *dist = t;
    return true;
  }
  return false;
}

bool scene_intersect(float3 *origin,
  float3 *direction,
  __global float *spheresData,
  int numSpheres,
  float3 *hit,
  float3 *normal,
  Material *material)
{
  float resDist = FLT_MAX;
  float spheres_dist = FLT_MAX;
  for (int i = 0; i < numSpheres; i++)
  {
    float dist_i;

    float3 spherePos = sphere_pos_vec3(i);
    if (sphere_ray_intersect(origin, direction, &spherePos, sphere_radius(i), &dist_i)
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
  resDist = spheres_dist;

  float triangle_dist = FLT_MAX;

  float3 vert0 = (float3)(955.0f, 550.0f, 18.0f);
  float3 vert1 = (float3)(950.0f, 535.0f, 13.0f);
  float3 vert2 = (float3)(960.0f, 535.0f, 13.0f);
  float t = 0;
  float3 N;
  if (triangle_ray_intersect(origin, direction, &vert0, &vert1, &vert2, &N, &t) && t < resDist)
  {
    triangle_dist = t;
    *normal = N;
    *hit = *origin + *direction * t;
    place_material(material, spheresData, 1);
  }
  resDist = min(resDist, triangle_dist);

  triangle_dist = FLT_MAX;
  vert0 = (float3)(952.0f, 550.0f, 42.0f);
  vert1 = (float3)(947.0f, 535.0f, 42.0f);
  vert2 = (float3)(957.0f, 535.0f, 42.0f);
  t = 0;
  if (triangle_ray_intersect(origin, direction, &vert0, &vert1, &vert2, &N, &t) && t < resDist)
  {
    triangle_dist = t;
    *normal = N;
    *hit = *origin + *direction * t;
    place_material(material, spheresData, 3);
  }
  resDist = min(resDist, triangle_dist);

  triangle_dist = FLT_MAX;
  vert0 = (float3)(960.0f, 550.0f, 80.0f);
  vert1 = (float3)(955.0f, 535.0f, 80.0f);
  vert2 = (float3)(965.0f, 535.0f, 80.0f);
  t = 0;
  if (triangle_ray_intersect(origin, direction, &vert0, &vert1, &vert2, &N, &t) && t < resDist)
  {
    triangle_dist = t;
    *normal = N;
    *hit = *origin + *direction * t;
    place_material(material, spheresData, 3);
  }
  resDist = min(resDist, triangle_dist);

  float plane_dist = FLT_MAX;
  N = (float3)(0.0f, 1.0f, 0.0f);
  if (plane_ray_intersect(origin, direction, (float3)(960.0f, 535.0f, 10.0f), &N, &t)
      && t < resDist)
  {
    plane_dist = t;
    *hit = *origin + *direction * plane_dist;
    *normal = N;

    material->diffuse_color = ((int)(.5 * hit->x + 1000) + (int)(.5 * hit->z)) & 1
                                ? (float3)(1, 1, 1)
                                : (float3)(1, .7, .3);

    material->diffuse_color = material->diffuse_color * 0.3f;
  }

  return min(resDist, plane_dist) < 1000;
}

float3 cast_ray(float3 *origin,
  float3 *direction,
  __global float *spheresData,
  int numSpheres,
  __global float *lightingData,
  int numLightSources,
  int depth)
{
  // intersection point and normal in that point
  float3 hit, normal;
  Material material;
  material_init(&material);

  if (depth > 3
      || !scene_intersect(origin, direction, spheresData, numSpheres, &hit, &normal, &material))
    return BACKGROUND_COLOR;

  float3 reflect_dir = normalize(reflect(direction, &normal));
  float3 refract_dir = normalize(refract(direction, &normal, &(material.refractive_index)));

  float3 reflect_origin =
    dot(reflect_dir, normal) < 0 ? hit - normal * epsilon : hit + normal * epsilon;
  float3 refract_origin =
    dot(refract_dir, normal) < 0 ? hit - normal * epsilon : hit + normal * epsilon;

  float3 reflect_color = 0;
  float3 refract_color = 0;
  if (material.albedo.z != 0.0f)
    reflect_color = cast_ray(&reflect_origin,
      &reflect_dir,
      spheresData,
      numSpheres,
      lightingData,
      numLightSources,
      depth + 1);

  if (material.albedo.w != 0.0f)
    refract_color = cast_ray(&refract_origin,
      &refract_dir,
      spheresData,
      numSpheres,
      lightingData,
      numLightSources,
      depth + 1);

  float diffuse_light_intensity = 0;
  float specular_light_intensity = 0;
  for (int j = 0; j < numLightSources; ++j)
  {
    // prabably should create the same structure, like for materials
    float3 ld = light_pos_vec3(j) - hit;
    float light_distance = length(ld);
    float3 light_dir = normalize(ld);

    float3 shadow_orig = 0;
    if (dot(light_dir, normal) < 0)
      shadow_orig = hit - normal * epsilon;
    else
      shadow_orig = hit + normal * epsilon;

    float3 shadow_pt, shadow_n;
    Material tmpMaterial;

    if (scene_intersect(
          &shadow_orig, &light_dir, spheresData, numSpheres, &shadow_pt, &shadow_n, &tmpMaterial)
        && length(shadow_pt - shadow_orig) < light_distance)
      continue;

    diffuse_light_intensity += lightingData[get_index_l(j, 3)] * max(0.0f, dot(light_dir, normal));
    specular_light_intensity +=
      pow(max(0.0f, dot(reflect(&light_dir, &normal), *direction)), material.specular_exponent)
      * lightingData[get_index_l(j, 3)];
  }

  return (float3)(material.diffuse_color * diffuse_light_intensity * material.albedo.x
                  + (float3)(1.0f, 1.0f, 1.0f) * specular_light_intensity * material.albedo.y
                  + reflect_color * material.albedo.z + refract_color * material.albedo.w);
}

__kernel void raytracer(__write_only image2d_t image,
  float3 cameraPos,
  float screenDistance,
  __global float *spheresData,
  int numSpheres,
  __global float *lightingData,
  int numLightSources)
{
  const int2 pos = (int2)(get_global_id(0), get_global_id(1));
  float3 dir = (float3)(pos.x + 0.5, pos.y + 0.5, screenDistance);
  dir -= cameraPos;
  dir = normalize(dir);
  float3 color =
    cast_ray(&cameraPos, &dir, spheresData, numSpheres, lightingData, numLightSources, 0);

  write_imagef(image, pos, (float4)(color, 1));
}
