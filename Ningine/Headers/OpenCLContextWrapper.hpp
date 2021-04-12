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

class OpenCLContextWrapper : public CLWrapper
{
public:
  OpenCLContextWrapper();
  ~OpenCLContextWrapper() = default;

  int init(GLFWwindow *glWindow, const std::string &kernelPath, uint glTextureID);
  void processKernel(cl::NDRange range);

  inline void updateScreenRange(uint2 size) { screenRange = { size.x, size.y }; }

  void createBuffer(vecf &data);
  
  cl::ImageGL &getScreenImage() { return glScreenImage; }

private:
  cl::NDRange screenRange;
  cl::CommandQueue commandQueue;

  // TODO: consider about moving fields that we are using for OpenCL-OpenGL interoperability 
  //        into separate class, something like a brigde between two technologies
  cl::ImageGL glScreenImage;
};
}
