#pragma once

#ifdef _WIN32
#pragma warning(disable : 4996)
#endif
#include <CL/cl.hpp>
#ifndef _WIN32
#include <CL/cl2.hpp>
#endif

<<<<<<< HEAD:tracer/include/CLError.hpp
=======
#define CL_UNKNOWN_BUILD_ERROR -9999
>>>>>>> experimental/test_raytracer:include/CLError.hpp
const char *getErrorString(cl_int error);
