#pragma once
#include "CLWrapper.hpp"
#include "Utility.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace ningine
{
void CLWrapper::printDeviceInfo(const std::vector<cl::Device> &devices)
{
  using namespace utility;
  typedef std::vector<cl::Device>::const_iterator DeviceIterator;

  std::cout << "number of devices  \t" << devices.size() << "\n\n";

  size_t i = 1;
  for (const cl::Device &device : devices) 
  {
    const std::string vendor = device.getInfo<CL_DEVICE_VENDOR>();
    const std::string version = device.getInfo<CL_DEVICE_VERSION>();
    const cl_ulong gmem = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();// in bytes
    const cl_ulong lmem = device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();// in bytes
    const cl_ulong cmem = device.getInfo<CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE>();// in bytes

    std::cout << "Device:         \t" << i++ << "\n";
    std::cout << "Vendor:         \t" << vendor << "\n";
    std::cout << "Version:        \t" << version << "\n";
    std::cout << "Global Memory:  \t" << to_GB(gmem) << " GB\n";
    std::cout << "Local Memory:   \t" << to_kB(lmem) << " kB\n";
    if (cmem >= GB)
      std::cout << "Constant Memory:\t" << to_GB(cmem) << " kB\n\n";
    else if (cmem >= MB)
      std::cout << "Constant Memory:\t" << to_MB(cmem) << " kB\n\n";
    else
      std::cout << "Constant Memory:\t" << to_kB(cmem) << " kB\n\n";
  }
}

std::string CLWrapper::parseLog(char *log)
{
  std::istringstream iss(log);
  std::string line;
  std::stringstream out;

  while (std::getline(iss, line))
  {
    // parse the lines
    // size_t found;
    if (line.find("note") != std::string::npos)
    {
      out << "\033[92m" << line << "\n";
    }
    else if (line.find("warning") != std::string::npos)
    {
      out << "\033[33m" << line << "\n";
    }
    else if (line.find("error") != std::string::npos)
    {
      out << "\033[31m" << line << "\n";
    }
    else
    {
      out << "\033[0m" << line << "\n";
    }
  }

  return out.str();
}

cl::Program CLWrapper::createCLProgram(const std::string &file, cl_context_properties *properties)
{
  for (int i = 0; i < 40; i++) std::cout << "-";
  std::cout << "\n";

  // TODO: check whether we could replace code below with already implemented utility "getPlatform()"
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);

  if (platforms.empty())
  {
    std::cerr << "platforms is empty" << std::endl;
    exit(1);
  }
  std::cout << "num platforms\t" << platforms.size() << "\n";

  auto platform = platforms.front();
  std::vector<cl::Device> devices;
  platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
  auto device = devices.front();

  if (devices.empty())
  {
    std::cerr << "devices is empty" << std::endl;
    exit(2);
  }

  printDeviceInfo(devices);

  for (int i = 0; i < 40; i++) std::cout << "-";
  std::cout << "\n";

  std::ifstream clFile(file);
  std::string src(std::istreambuf_iterator<char>(clFile), (std::istreambuf_iterator<char>()));

  cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));
  context = cl::Context(device, properties);
  program = cl::Program(context, sources);

  auto error = program.build("-cl-std=CL1.2");

  if (error == CL_BUILD_PROGRAM_FAILURE || error == CL_UNKNOWN_BUILD_ERROR)
  {
    size_t logSize;
    clGetProgramBuildInfo(program(), device(), CL_PROGRAM_BUILD_LOG, 0, nullptr, &logSize);

    char *log = new char[logSize];

    clGetProgramBuildInfo(program(), device(), CL_PROGRAM_BUILD_LOG, logSize, log, nullptr);

    std::cerr << "OpenCL build error:\n" << parseLog(log) << std::endl;
    exit(error);
  }
  else if (error)
  {
    std::cerr << "OpenCL error:\t" << getErrorString(error) << std::endl;
    exit(error);
  }

  return program;
}

void CLWrapper::init(std::string path, cl_context_properties *properties)
{
  createCLProgram(path, properties);
  device = context.getInfo<CL_CONTEXT_DEVICES>().front();
}

}// namespace ningine
