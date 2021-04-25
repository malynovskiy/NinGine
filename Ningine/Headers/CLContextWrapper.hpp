#pragma once

#include "NingineInit.hpp"
#include "CLError.hpp"
#include "GLContextWrapper.hpp"

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

class CLContextWrapper
{
public:
  CLContextWrapper() = default;
  ~CLContextWrapper() = default;

  int init(GLFWwindow *glWindow, const std::string &kernelPath, uint glTextureID);
  void processKernel(cl::NDRange range);

  inline void updateScreenRange(uint2 size) { screenRange = { size.x, size.y }; }
  
  cl::ImageGL &getScreenImage() { return glScreenImage; }

  inline void clearBuffers() { memBuff.clear(); }

  void createBuffer(vecf &data);
  inline void createBuffer(
    cl_mem_flags flags, ::size_t size, void *host_ptr = nullptr, cl_int *err = nullptr)
  {
    memBuff.push_back(cl::Buffer(context, flags, size, host_ptr, err));
  }

  inline void createKernel(std::string name, cl_int *err = nullptr)
  {
    kernel = cl::Kernel(program, name.c_str(), err);
  }

  template<typename T>
  inline void setKernelArgs(cl_uint index, const T &value)
  {
    kernel.setArg(index, value);
  }

  inline const cl::Program &getProgram() const { return program; }
  inline const cl::Context &getContext() const { return context; }
  inline const cl::Device &getDevice() const { return device; }
  inline const cl::Buffer &getBuffer(size_t i = 0) const { return memBuff.at(i); }
  inline const cl::Kernel &getKernel() const { return kernel; }

  void printDeviceInfo(const std::vector<cl::Device> &devices);
  static std::string parseLog(char *log);
private:
  cl::Program createCLProgram(const std::string &file, cl_context_properties *properties = nullptr);

private:
  cl::NDRange screenRange;
  cl::CommandQueue commandQueue;

  // TODO: consider about moving fields that we are using for OpenCL-OpenGL interoperability 
  //        into separate class, something like a brigde between two technologies
  cl::ImageGL glScreenImage;

  cl::Program program;
  cl::Context context;
  cl::Device device;

  std::vector<cl::Buffer> memBuff;
  cl::Kernel kernel;
};
}
