#include "OpenCLContextWrapper.hpp"
#include "OpenCLUtility.hpp"

#include <iostream>

namespace ningine
{
OpenCLContextWrapper::OpenCLContextWrapper() {}

int OpenCLContextWrapper::init(GLFWwindow *glWindow, const std::string &kernelPath, uint glTextureID)
{
  assert(glWindow != nullptr);

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

void OpenCLContextWrapper::processKernel(cl::NDRange range)
{
  commandQueue = cl::CommandQueue(m_clWrapper.getContext(), m_clWrapper.getDevice());
  std::vector<cl::Memory> images(1, glScreenImage);
  
  // tell openGL to let openCL use the memory
  commandQueue.enqueueAcquireGLObjects(&images, nullptr);
  screenRange = range;
  commandQueue.enqueueNDRangeKernel(m_clWrapper.getKernel(), 0, screenRange);
  // give the memory back to openGL
  commandQueue.enqueueReleaseGLObjects(&images, nullptr);
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
