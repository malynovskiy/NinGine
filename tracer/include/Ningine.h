#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <random>
#include <map>

namespace ningine
{

class Ningine
{
  GLFWwindow *window;

  std::uniform_real_distribution<float> randDistribution;

public:
  static int screenWidth;
  static int screenHeight;

  static std::map<int, bool> keyMap;

public:
  bool init();
  void run();


  bool createGLContext();

  static void resize_callback(GLFWwindow *window, int width, int height);
  static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
};

}// namespace ningine
