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

  // TODO: when CLWrapper would be merged with this class this stubs should be dropped
  void createBuffer(vecf &data);
  void clearBuffers();

private:
  CLWrapper m_clWrapper;

  // TODO: consider about moving fields that we are using for OpenCL-OpenGL interoperability 
  //        into separate class, something like a brigde between two technologies
  cl::ImageGL glScreenImage;
};
}
