#pragma once
#include <glm/glm.hpp>

namespace ningine
{
class Camera
{
  glm::vec3 position;
  glm::vec3 forward;
  glm::vec3 up;
  glm::vec3 right;
};
}// namespace ningine
