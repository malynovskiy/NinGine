#pragma once

#include <vector>
#include "CLWrapper.hpp"

#ifdef __WIN64
    #define WINDOWS
#endif

#ifdef __linux__
    #define LINUX
#endif

#ifdef __MACH__
    #define MAC
#endif

namespace ningine
{
#ifdef LINUX || MAC
typedef __int64_t int64;
#endif

#ifdef WINDOWS
typedef __int64 int64;
#endif

typedef std::vector<float> vecf;
typedef unsigned int uint;
typedef cl_float3 float3;
typedef cl_uint2 uint2;

constexpr char RaytracerKernelName[] = "raytracer";

}// namespace ningine
