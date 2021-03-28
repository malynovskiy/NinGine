#pragma once
#include <glad/glad.h>

#include "OpenGLContextWrapper.hpp"
#include "OpenCLContextWrapper.hpp"

#include <random>
#include <map>

#include "Scene.hpp"

namespace ningine
{

constexpr char glShaderProgramName[] = "BasicView";
constexpr char vertexShaderName[] = "../../../source/shaders/GLSL/basic.vert";
constexpr char fragmentShaderName[] = "../../../source/shaders/GLSL/basic.frag";

constexpr char raytracer_kernel_path[] = "../../../source/shaders/OpenCL/ray_tracer.cl";

// start coodinate for the first sphere that could be added in runtime
constexpr glm::vec3 start_sphere_pos = glm::vec3(607, 345, 70);
constexpr glm::vec3 spheres_offset = glm::vec3(15, 15, 15);

class Ningine
{
public:
  Ningine() {};
  ~Ningine() {}

private:
  void initKeyboardMappings();
  void initGLTexture();
  bool initCLContext();

  void createScene();

  void createSpheres();
  void createTriangles();
  void createPlanes();

  void spheresMove();

  void createLighting();
  void createCLKernels();

  void processKeyboardInput();

  void pushBackSphere(const Sphere &sphere);
  void pushBackLightSource(const LightSource &lightSource);

  float calculateDist(float fov);
  float calculateFOV(glm::vec2 a, glm::vec2 b, glm::vec2 c);

public:
  bool init();
  int run();
  void display();
  void processEvents();

  bool createGLContext();
  void createScreenImage();

  void printFrameRate();

  static void resize_callback(GLFWwindow *window, int width, int height);
  static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

private:
  GLFWwindow *window;

  std::random_device randGen;
  std::uniform_real_distribution<float> randDistribution;

  GLShaderProgram shaderProgram;
  GLScreen screenPlane;
  CLWrapper clContext;

  /* TODO: Move all OpenCL info into separate class -> wrapper for OpenCL context: 
            maybe we should extend already existed class CLWrapper
    1) need to be investigated whether we want to putt all OpenCL context info into CLWrapper
        > if no then create separete class, that would be containing clContext and all related 
          info*/

  /* TODO: probably some similar stuff, but for OpenGL context */
  cl::ImageGL screenImage;
  float screenDistance;
  cl::NDRange screenRange;
  cl::CommandQueue commandQueue;

  GLuint textureID;

  std::vector<float> spheres;
  std::vector<float> lightSources;

  int numberOfSpheres;
  int numberOfLightSources;

  cl_float3 camPos;
  cl_float3 coordinateBasis;
  // coordinates for adding new spheres in run-time
  glm::vec3 curr_coordinate;
  glm::vec3 spherePos;

  const float fov = 60.0f;
  const uint attrsPerSphere = 13;
  const uint attrsPerLightSource = 4;
 
  static int screenWidth;
  static int screenHeight;
  static std::map<int, bool> keyMap;
};

}// namespace ningine
