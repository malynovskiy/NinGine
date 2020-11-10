#pragma once

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#include "GLShaderProgram.hpp"
#include "GLScreen.hpp"
#include "CLWrapper.hpp"

#include <random>
#include <map>

namespace ningine
{

constexpr char glShaderProgramName[] = "BasicView";
constexpr char vertexShaderName[] = "../../../source/shaders/GLSL/basic.vert";
constexpr char fragmentShaderName[] = "../../../source/shaders/GLSL/basic.frag";

// start coodinate for the first sphere that could be added in runtime
constexpr glm::vec3 start_sphere_pos = glm::vec3(607, 345, 70);
constexpr glm::vec3 spheres_offset = glm::vec3(15, 15, 15);

class Ningine
{
  GLFWwindow *window;

  std::random_device randGen;
  std::uniform_real_distribution<float> randDistribution;

  GLShaderProgram program;
  GLScreen screenPlane;
  CLWrapper clContext;

  cl::ImageGL screenImage;
  float screenDistance;
  cl::NDRange screenRange;
  cl::CommandQueue queue;

  GLuint textureID;

  std::vector<float> spheres;
  int numberOfSpheres;

  static int screenWidth;
  static int screenHeight;
  static std::map<int, bool> keyMap;

  cl_float3 camPos;

  // coordinates for adding new spheres in run-time
  glm::vec3 curr_coordinate;
  glm::vec3 spherePos;
  const u_int attrsPerSphere = 20;

private:
  void initKeyMappings();
  void initGLTexture();
  bool initCLContext();

  // TODO: Ivestigete whether we can drop the next function
  //      and create some sort of scenes (width preloaded objects)
  void createSpheres();
  void createCLKernels();

  void processKeyboardInput();

  void addSphere(glm::vec3 pos,
    float r,
    glm::vec3 colour,
    glm::vec3 lightAmbiant,
    glm::vec3 lightSpecular,
    glm::vec3 materialAmbiant,
    glm::vec3 materialDiffuse,
    glm::vec3 materialSpecular,
    float materialShinyness,
    float reflective,
    float opacity,
    float refractiveIndex);

  float calculateDist(float fov);
  float calculateFOV(glm::vec2 a, glm::vec2 b, glm::vec2 c);

public:
  bool init();
  int run();
  void display();
  void processEvents();

  bool createGLContext();
  void createScreenImage();

  static void resize_callback(GLFWwindow *window, int width, int height);
  static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
};

}// namespace ningine
