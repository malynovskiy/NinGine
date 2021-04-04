#include "Scene.hpp"
#include "Vector.hpp"

#include <iostream>

namespace ningine
{
Scene::Scene()
  : cameraPosition(), coordinateBasis(), fov(DefaultFieldOfView), numberOfLightSources(0),
    numberOfSpheres(0)
{
  coordinateBasis = cameraPosition = float3{ 0.0f, 0.0f, 0.0f };
}

Scene &Scene::operator=(Scene &&other) noexcept
{
  if (this == &other)
    return *this;

  m_spheres = std::exchange(other.m_spheres, {});
  m_light_sources = std::exchange(other.m_light_sources, {});

  coordinateBasis = std::exchange(other.coordinateBasis, {});
  cameraPosition = std::exchange(other.cameraPosition, {});

  numberOfSpheres = other.numberOfSpheres;
  numberOfLightSources = other.numberOfLightSources;

  fov = other.fov;

  return *this;
}

void Scene::create(const float3 &CameraPosition, const float3 &CoordinateBasis, uint2 windowSize)
{
  cameraPosition = CameraPosition;
  coordinateBasis = CoordinateBasis;

  createObjects();
  createLighting();

  screenDistance = calcScreenDistance(fov, windowSize);

  // TODO: should be printed only for Debug build
  std::cout << "screenDist\t" << screenDistance << '\n';
  /*std::cout << "FOV:\t"
            << calcFOV(glm::vec2(screenHeight, 0),
                 glm::vec2(0, screenDistance),
                 glm::vec2(screenWidth, screenDistance));*/
  std::cout << "FOV:\t" << fov;
  std::cout << (char)167 << '\n';
}

void Scene::createObjects()
{
  createSpheres();
  createTriangles();
}

void Scene::createSpheres()
{
  using math::Vec3f;
  numberOfSpheres = 0;

  const Vec3f basis(coordinateBasis.x, coordinateBasis.y, coordinateBasis.z);

  putSphere(Sphere(basis + Vec3f(5, 0, 10 + 10), 2, materials::ivory));
  // putSphere(Sphere(basis + Vec3f(-1.0, -1.5, 12 + 40), 2, materials::thin_glass));

  putSphere(Sphere(basis + Vec3f(1.5, -0.5, 18 + 40), 3, materials::red_rubber));
  putSphere(Sphere(basis + Vec3f(7, 5, 18 + 50), 4, materials::mirror));

  // 3
  putSphere(Sphere(basis + Vec3f(20, -1.0, 80),
    4,
    Material(math::Vec3f(0.0f, 0.0f, 0.0f), math::Vec4f(0.9f, 0.1f, 0.0f, 0.0f), 5.0f, 1.0f)));
  // 4
  putSphere(Sphere(basis + Vec3f(26, 1, 45),
    2,
    Material(math::Vec3f(0.0f, 0.99f, 0.2f), math::Vec4f(0.9f, 0.1f, 0.0f, 0.0f), 5.0f, 1.0f)));
  // 5
  putSphere(Sphere(basis + Vec3f(32, -0.5, 60),
    2.5,
    Material(math::Vec3f(0.1f, 0.1f, 0.99f), math::Vec4f(0.9f, 0.1f, 0.0f, 0.0f), 5.0f, 1.0f)));

  // putSphere(Sphere(basis + Vec3f(1.5, -0.5, 18 + 40),
  //  1,
  //  Material(math::Vec3f(0.72f, 0.08f, 0.99f), math::Vec4f(0.9f, 0.1f, 0.0f, 0.0f), 5.0f, 1.0f)));

  // putSphere(Sphere(basis + Vec3f(1.5, -0.5, 18 + 40),
  //  4,
  //  Material(math::Vec3f(0.99f, 0.08f, 0.6f), math::Vec4f(0.9f, 0.1f, 0.0f, 0.0f), 5.0f, 1.0f)));

  // putSphere(Sphere(basis + Vec3f(1.5, -0.5, 18 + 40),
  //  3,
  //  Material(math::Vec3f(0.22f, 0.58f, 0.55f), math::Vec4f(0.9f, 0.1f, 0.0f, 0.0f), 5.0f, 1.0f)));

  // putSphere(Sphere(basis + Vec3f(1.5, -0.5, 18 + 40),
  //  4,
  //  Material(math::Vec3f(0.93f, 0.46f, 0.23f), math::Vec4f(0.9f, 0.1f, 0.0f, 0.0f), 5.0f, 1.0f)));


  m_spheres.shrink_to_fit();

  // TODO: Replace this code later to a more robust approach of moving "lead" sphere
  /*spherePos.x = spheres.at(0);
  spherePos.y = spheres.at(1);
  spherePos.z = spheres.at(2);*/
}

void Scene::createTriangles()
{
  using math::Vec3f;
  // numberOfTriangles = 0;

  const Vec3f basis(coordinateBasis.x, coordinateBasis.y, coordinateBasis.z);
}

void Scene::createLighting()
{
  numberOfLightSources = 0;

  const math::Vec3f basis(coordinateBasis.x, coordinateBasis.y, coordinateBasis.z);

  // probably need more intensity
  putLightSource(LightSource(basis + math::Vec3f(-20.0f, 30.0f, 20.0f), 3.5f));
  putLightSource(LightSource(basis + math::Vec3f(20.0f, 20.0f, 30.0f), 1.5f));
  m_light_sources.shrink_to_fit();
}

void Scene::putSphere(const Sphere &sphere)
{
  m_spheres.push_back(sphere.center.x);
  m_spheres.push_back(sphere.center.y);
  m_spheres.push_back(sphere.center.z);

  m_spheres.push_back(sphere.radius);

  m_spheres.push_back(sphere.material.diffuse_color.x);
  m_spheres.push_back(sphere.material.diffuse_color.y);
  m_spheres.push_back(sphere.material.diffuse_color.z);

  m_spheres.push_back(sphere.material.albedo.x);
  m_spheres.push_back(sphere.material.albedo.y);
  m_spheres.push_back(sphere.material.albedo.z);
  m_spheres.push_back(sphere.material.albedo.w);

  m_spheres.push_back(sphere.material.specular_exponent);
  m_spheres.push_back(sphere.material.refractive_index);

  numberOfSpheres++;

  assert(m_spheres.size() / numberOfSpheres == attrsPerSphere);
}

void Scene::putLightSource(const LightSource &lightSource)
{
  m_light_sources.push_back(lightSource.position.x);
  m_light_sources.push_back(lightSource.position.y);
  m_light_sources.push_back(lightSource.position.z);

  m_light_sources.push_back(lightSource.intensity);

  numberOfLightSources++;
  assert(m_light_sources.size() / numberOfLightSources == attrsPerLightSource);
}

}
