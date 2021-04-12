#pragma once

#include <vector>
#include "CLWrapper.hpp"

#if defined(_WIN32)
  #define WINDOWS
#elif defined(__linux__)
  #define LINUX
#elif defined(__MACH__)
  #define MAC
#endif

namespace ningine
{

#if defined(WINDOWS)
typedef __int64 int64;
#else
typedef __int64_t int64;
#endif

typedef std::vector<float> vecf;
typedef unsigned int uint;
typedef cl_float3 float3;
typedef cl_uint2 uint2;
typedef cl_ulong uint64;
constexpr char RaytracerKernelName[] = "raytracer";

}// namespace ningine
