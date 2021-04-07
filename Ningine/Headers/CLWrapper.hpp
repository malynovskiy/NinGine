#pragma once

#include <CLError.hpp>

namespace ningine
{
class CLWrapper
{
public:
  CLWrapper() {}
  ~CLWrapper() {}

  void init(std::string path, cl_context_properties *properties = nullptr);

  inline void clearBuffers() { memBuff.clear(); }

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

private:
  cl::Program createCLProgram(const std::string &file, cl_context_properties *properties = nullptr);
  void printDeviceInfo(const std::vector<cl::Device> &devices);
  static std::string parseLog(char *log);

private:
  cl::Program program;
  cl::Context context;
  cl::Device device;

  std::vector<cl::Buffer> memBuff;
  cl::Kernel kernel;
};

}// namespace ningine
