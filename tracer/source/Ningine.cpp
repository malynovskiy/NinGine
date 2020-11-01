#include "Ningine.h"

#include <iostream>
#include <iomanip>

namespace ningine
{

int Ningine::screenWidth = 1366;
int Ningine::screenHeight = 768;

std::map<int, bool> Ningine::keyMap;

bool Ningine::createGLContext()
{
  if (!glfwInit())
  {
    std::cerr << "Error, failed to init glfw: 0x" << std::hex << std::setw(8) << std::setfill('0')
              << glfwGetError(nullptr) << '\n';
    return false;
  }

  // TODO: verify do we need the next hint at all?
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  // TODO: probably the next hint will be useful for fps capabilities
  glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

  window = glfwCreateWindow(screenWidth, screenHeight, "real-time Ray-Tracing test", nullptr, nullptr);
  if (window == nullptr)
  {
    std::cerr << "Error, failed to create glfw window!\n";
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Error, failed to load the GLAD!\n";
    glfwTerminate;
    return false;
  }

  std::cout << "OpenGL version is: " << glGetString(GL_VERSION) << '\n';

  return true;
}

bool Ningine::init()
{
  if (!createGLContext())
    return false;

  randDistribution = std::uniform_real_distribution<float>(0.0f, 1.0f);

  glfwSetWindowSizeCallback(window, resize_callback);
  glfwSetKeyCallback(window, key_callback);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}


void Ningine::resize_callback(GLFWwindow *window, int width, int height)
{
  screenWidth = width;
  screenHeight = height;

  glViewport(0, 0, width, height);
}

void Ningine::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  try
  {
    if (action == GLFW_PRESS)
      keyMap.at(key) = true;
    else if (action == GLFW_RELEASE)
      keyMap.at(key) = false;
  }
  catch (...)
  {
  }
}

}// namespace ningine
