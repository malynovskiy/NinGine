#pragma once

#ifdef _WIN32
#pragma warning(disable : 4996)
#endif

#include <CL/cl2.hpp>

#define CL_UNKNOWN_BUILD_ERROR -9999
std::string getErrorString(cl_int errorCode);
