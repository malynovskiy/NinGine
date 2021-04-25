#pragma once
#include <glad/glad.h>

#include "GLContextWrapper.hpp"
#include "CLContextWrapper.hpp"
#include "GraphicsContextBridge.hpp"

#include <random>
#include <map>

#include "Scene.hpp"

namespace ningine
{

constexpr char raytracer_kernel_path[] = "../../../source/shaders/OpenCL/ray_tracer.cl";

// start coodinate for the first sphere that could be added in runtime
constexpr glm::vec3 start_sphere_pos = glm::vec3(607, 345, 70);
constexpr glm::vec3 spheres_offset = glm::vec3(15, 15, 15);

class Ningine
{
public:
  Ningine() = default;
  ~Ningine() = default;

private:
  void spheresMove();

  void createCLKernels();

  void processKeyboardInput();

  void render();
  void processEvents();

  void printFrameRate();

public:
  bool init();
  int run();

private:
  //std::random_device randGen;
  //std::uniform_real_distribution<float> randDistribution;

  GLContextWrapper m_glContext;
  CLContextWrapper m_clContext;
  
  //GraphicsContextBridge m_graphicsContexBridge;

  Scene m_scene;

  /* TODO: Move all OpenCL info into separate class -> wrapper for OpenCL context: 
            maybe we should extend already existed class CLWrapper
    1) need to be investigated whether we want to putt all OpenCL context info into CLWrapper
        > if no then create separete class, that would be containing clContext and all related 
          info*/

  /* TODO: probably some similar stuff, but for OpenGL context */

  // coordinates for adding new spheres in run-time
  glm::vec3 curr_coordinate;
  glm::vec3 spherePos;
};

}// namespace ningine
