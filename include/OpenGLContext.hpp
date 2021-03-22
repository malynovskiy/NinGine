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

constexpr char* WindowName = "Ningine Ray-Tracing";

namespace
{
}

namespace ningine
{
  class OpenGLContext
  {
  public:
    OpenGLContext();
    ~OpenGLContext() {}
  
    bool init();

    uint getWindowHeight() { return windowHeight; }
    uint getWindowWidth() { return windowWidth; }

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

    uint windowHeight, windowWidth;
  };
}// namespace ningine
