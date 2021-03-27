#include "OpenCLContext.hpp"
#include "OpenCLUtility.hpp"

#include <iostream>

namespace ningine
{
OpenCLContext::OpenCLContext() {}

int OpenCLContext::init(GLFWwindow *glWindow, const std::string &kernelPath, uint glTextureID)
{
  cl_int result = CL_SUCCESS;

  cl::Platform clPlatform = getPlatform();
  constexpr size_t NumberOfContextProperties = 6 + 1; // + 1 because should terminate with 0 
  std::array<cl_context_properties, NumberOfContextProperties> contextProperties = {
    CL_GL_CONTEXT_KHR, OpenGLContext::getPlatformGLNativeContext(glWindow),
    NativeDisplayProperty, OpenGLContext::getPlatformGLNativeWindow(glWindow),
    CL_CONTEXT_PLATFORM, (cl_context_properties)clPlatform(),
    0 
  };

  m_clWrapper.init(kernelPath, contextProperties.data());

  glScreenImage = cl::ImageGL(
    m_clWrapper.getContext(), CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, glTextureID, &result);

  if (result != CL_SUCCESS)
  {
    std::cerr << "Error, cl::ImageGL was not created:\t" << getErrorString(result) << std::endl;
    return result;
  }

  m_clWrapper.createKernel(RaytracerKernelName);

  return result;
}
}// namespace ningine
