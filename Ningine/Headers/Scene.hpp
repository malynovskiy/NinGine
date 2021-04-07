#pragma once
#include "NingineInit.hpp"
#include "Sphere.hpp"
#include "Light.hpp"

#include <glm/glm.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

namespace ningine
{
constexpr float DefaultFieldOfView = 60.0f;

// TODO: should be moved into future Camera class
inline float calcScreenDistance(float fov, uint2 screenSize)
{
  return static_cast<float>((screenSize.x / 2.0f) / tan(screenSize.y / 2.0 * M_PI / 180.0f));
}

inline float calcFieldOfView(glm::vec2 a, glm::vec2 b, glm::vec2 c)
{
  using namespace glm;

  const vec2 v1 = normalize(b - a);
  const vec2 v2 = normalize(c - a);
  return static_cast<float>(acos(dot(v1, v2)) * 180 / M_PI);
}

class Scene
{
public:
  Scene();
  Scene &operator=(Scene &&other) noexcept;
  ~Scene() {}

  void create(const float3 &CameraPosition, const float3 &CoordinateBasis, uint2 windowSize);

  vecf &getObjects() { return m_spheres; }
  vecf &getLights() { return m_light_sources; }
  float3 &getCameraPosition() { return cameraPosition; }
  float getScreenDistance() { return screenDistance; }
  uint getNumberOfSpheres() { return numberOfSpheres; }
  uint getNumberOfLights() { return numberOfLightSources; }

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
  float screenDistance;

  const uint attrsPerSphere = 13;
  const uint attrsPerLightSource = 4; 
};
}
