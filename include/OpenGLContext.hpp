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

#include <map>


namespace ningine
{
constexpr char *WindowName = "Ningine Ray-Tracing";
constexpr uint2 DefaultWindowSize = { 1366, 768 };

class OpenGLContext
{
public:
  OpenGLContext();
  ~OpenGLContext() {}

  bool init();
  void initTexture();

  uint getWindowHeight() { return windowSize.y; }
  uint getWindowWidth() { return windowSize.x; }

private:
  bool createContext();
  bool constructShaders();
  void initKeyMappings();

public:
  static void resize_callback(GLFWwindow *window, int width, int height);
  static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

private:
  static std::map<int, bool> keyMap;

private:
  GLFWwindow *m_window;
  GLShaderProgram m_shaderProgram;
  GLScreen m_screenPlane;

  uint textureID;
  uint2 windowSize;
};
}// namespace ningine
