#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GLShaderProgram.hpp"
#include "GLScreen.hpp"
#include "CLWrapper.hpp"

#include <random>
#include <map>

namespace ningine
{

constexpr char glShaderProgramName[] = "BasicView";
constexpr char vertexShaderName[] = "shaders/basic.vert";
constexpr char fragmentShaderName[] = "shaders/basic.frag";

class Ningine
{
  GLFWwindow *window;

  std::uniform_real_distribution<float> randDistribution;

  GLShaderProgram program;
  GLScreen screenPlane;
  CLWrapper clContext;

  float screenDistance;

  GLuint textureID;

  std::vector<float> spheres;
  int numberOfSpheres;

  static int screenWidth;
  static int screenHeight;
  static std::map<int, bool> keyMap;

  glm::vec3 spherePos;

  cl_float3 camPos;

private:
  void initKeyMappings();
  void initGLTexture();
  bool initCLContext();

  // TODO: Ivestigete whether we can drop the next function
  //      and create some sort of scenes (width preloaded objects)
  void createSpheres();
  void createCLKernels();

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
