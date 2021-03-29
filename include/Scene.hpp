#pragma once
#include "NingineInit.hpp"
#include "Sphere.hpp"
#include "Light.hpp"

namespace ningine
{
constexpr float DefaultFieldOfView = 60.0f;

class Scene
{
public:
  Scene();
  Scene &operator=(Scene &&other) noexcept;
  ~Scene() {}

  void create(const float3 &CameraPosition, const float3 &CoordinateBasis);

  vecf& getObjects();
  vecf& getLights();

  void putSphere(const Sphere &sphere);
  void putLightSource(const LightSource &lightSource);
private:
  void createObjects();
  void createSpheres();
  void createTriangles();
  void createLighting();
  
private:
// for now we are storing scene data just as a vectors of floats
// TODO: should be revisited at some point
  vecf m_spheres;
  vecf m_light_sources;

  uint numberOfSpheres;
  uint numberOfLightSources;

  float3 cameraPosition;
  float3 coordinateBasis;

  // TODO: should be moved into Camera class
  float fov;

  const uint attrsPerSphere = 13;
  const uint attrsPerLightSource = 4; 
};
}
