#include "OpenCLContext.hpp"
#include "OpenCLUtility.hpp"
#include "OpenGLContext.hpp"

#include <iostream>

namespace ningine
{
OpenCLContext::OpenCLContext() {}

int OpenCLContext::init(GLFWwindow *glWindow, const std::string &kernelPath, uint glTextureID)
{
  cl_int result = CL_SUCCESS;

  cl::Platform clPlatform = getPlatform();
  cl_context_properties contextProperties[] = { CL_GL_CONTEXT_KHR,
    (cl_context_properties)glfwGetWGLContext(glWindow),
    CL_WGL_HDC_KHR,
    (cl_context_properties)GetDC(glfwGetWin32Window(glWindow)),
    CL_CONTEXT_PLATFORM,
    (cl_context_properties)clPlatform(),
    0 };

  m_clWrapper.init(kernelPath, contextProperties);

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
