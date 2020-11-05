#pragma once

#ifdef _WIN32
#pragma warning(disable : 4996)
#endif
#include <CL/cl.hpp>
#ifndef _WIN32
#include <CL/cl2.hpp>
#endif

const char *getErrorString(cl_int error);