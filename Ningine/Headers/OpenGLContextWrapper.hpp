#pragma once
#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "NingineInit.hpp"

#ifdef WINDOWS
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#ifdef LINUX
#define GLFW_EXPOSE_NATIVE_GLX
#define GLFW_EXPOSE_NATIVE_X11
#endif

#ifdef MAC
// TODO: check whether it's working
#define GLFW_EXPOSE_NATIVE_NSGL
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3native.h>

#include "GLShaderProgram.hpp"
#include "GLScreen.hpp"

#include <map>


namespace ningine
{
constexpr char *WindowName = "Ningine Ray-Tracing";
constexpr uint2 DefaultWindowSize = { 1366, 768 };

class OpenGLContextWrapper
{
public:
  OpenGLContextWrapper();
  ~OpenGLContextWrapper();

  bool init();
  uint initTexture();

  bool shouldCloseWindow();

  void render();
  void post_render();

  void pollEvents();

  uint2 getWindowSize() { return windowSize; }
  uint getWindowHeight() { return windowSize.y; }
  uint getWindowWidth() { return windowSize.x; }

  GLFWwindow *getWindowHandle() { return m_window; }

private:
  bool createContext();
  bool constructShaders();
  void initKeyMappings();

public:
  static void resize_callback(GLFWwindow *window, int width, int height);
  static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

  static intptr_t getPlatformGLNativeContext(GLFWwindow *window);
  static intptr_t getPlatformGLNativeWindow(GLFWwindow *window);
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
