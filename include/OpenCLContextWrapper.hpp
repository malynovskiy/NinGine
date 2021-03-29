#pragma once

#include "CLWrapper.hpp"
#include "NingineInit.hpp"
#include "OpenGLContextWrapper.hpp"

namespace ningine
{
constexpr cl_context_properties NativeDisplayProperty = 
#if defined(WINDOWS) 
                                                        CL_WGL_HDC_KHR; 
#elif defined(LINUX)
                                                        CL_GLX_DISPLAY_KHR;
#else
//TODO: Mac version probable should be addressed here
                                                        0;
#endif

class OpenCLContextWrapper
{
public:
  OpenCLContextWrapper();
  ~OpenCLContextWrapper() {}

  int init(GLFWwindow *glWindow, const std::string &kernelPath, uint glTextureID);

  // temporary stub for now
  void createBuffer(vecf &data);

private:
  CLWrapper m_clWrapper;

  cl::ImageGL glScreenImage;
};
}
