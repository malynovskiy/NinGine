#include "OpenCLContextWrapper.hpp"
#include "OpenCLUtility.hpp"

#include <iostream>

namespace ningine
{
OpenCLContextWrapper::OpenCLContextWrapper() {}

int OpenCLContextWrapper::init(GLFWwindow *glWindow, const std::string &kernelPath, uint glTextureID)
{
  cl_int result = CL_SUCCESS;

  cl::Platform clPlatform = getPlatform();
  ContextProperties contextProperties = {
    CL_GL_CONTEXT_KHR, OpenGLContextWrapper::getPlatformGLNativeContext(glWindow),
    NativeDisplayProperty, OpenGLContextWrapper::getPlatformGLNativeWindow(glWindow),
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

void OpenCLContextWrapper::createBuffer(vecf &data) 
{
  m_clWrapper.createBuffer(CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
    sizeof(float) * data.size(),
    data.data());
}

void OpenCLContextWrapper::clearBuffers()
{
  m_clWrapper.clearBuffers();
}

}// namespace ningine
