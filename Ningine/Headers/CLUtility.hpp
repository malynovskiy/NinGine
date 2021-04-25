#pragma once

#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
//#define CL_HPP_TARGET_OPENCL_VERSION 120

// #ifndef _WIN32 
// #include <CL/cl2.hpp>
// #else
// #pragma warning(disable : 4996)
// #endif// !WIN32

#include <CL/cl.hpp>
#include <string>

namespace ningine
{
typedef std::vector<cl_context_properties> ContextProperties;

static const std::string NVIDIA_PLATFORM = "NVIDIA";
static const std::string AMD_PLATFORM = "AMD";
static const std::string INTEL_PLATFORM = "Intel";
static const std::string MESA_PLATFORM = "Clover";
static const std::string APPLE_PLATFORM = "Apple";
cl::Platform getPlatform(std::string pName, cl_int &error);

cl::Platform getPlatform();

bool checkExtnAvailability(cl::Device pDevice, std::string pName);

cl::Program getProgram(cl::Context pContext, std::string file, cl_int &error);
}// namespace ningine
