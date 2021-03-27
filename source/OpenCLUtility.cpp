#include "OpenCLUtility.hpp"
#include "NingineInit.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace cl;
typedef std::vector<Platform>::iterator PlatformIter;

namespace ningine
{

Platform getPlatform(std::string pName, cl_int &error)
{
  Platform ret_val{};
  error = 0;
  try
  {
    std::vector<Platform> platforms;
    Platform::get(&platforms);
    
    int64 found = -1;
    for (PlatformIter it = platforms.begin(); it < platforms.end(); ++it)
    {
      std::string temp = it->getInfo<CL_PLATFORM_NAME>();
      if (temp.find(pName) != std::string::npos)
      {
        found = it - platforms.begin();
        std::cout << "Found platform: " << temp << std::endl;
        break;
      }
    }
    if (found == -1)
      error = +1;
    else
      ret_val = platforms[found];
  }
  catch (...)
  {
    std::cerr << "An error occured" << std::endl;
  }
  return ret_val;
}

#define FIND_PLATFORM(name)          \
  plat = getPlatform(name, errCode); \
  if (errCode == CL_SUCCESS)         \
    return plat;

Platform getPlatform()
{
  cl_int errCode;
  Platform plat;

  FIND_PLATFORM(NVIDIA_PLATFORM)
  FIND_PLATFORM(AMD_PLATFORM)
  FIND_PLATFORM(INTEL_PLATFORM)
  FIND_PLATFORM(APPLE_PLATFORM)
  FIND_PLATFORM(MESA_PLATFORM)

  // If no platforms are found
  exit(252);
}

bool checkExtnAvailability(Device pDevice, std::string pName)
{
  bool ret_val = true;
  try
  {
    std::string exts = pDevice.getInfo<CL_DEVICE_EXTENSIONS>();
    std::stringstream ss(exts);
    std::string item;
    int found = -1;
    while (std::getline(ss, item, ' '))
    {
      if (item == pName)
      {
        found = 1;
        break;
      }
    }
    if (found == 1)
    {
      std::cout << "Found CL_GL_SHARING extension: " << item << std::endl;
      ret_val = true;
    }
    else
    {
      std::cout << "CL_GL_SHARING extension not found\n";
      ret_val = false;
    }
  }
  catch (...)
  {
    std::cerr << "An error occured" << std::endl;
  }
  return ret_val;
}

Program getProgram(Context pContext, std::string file, cl_int &error)
{
  Program ret_val;
  error = 0;
  std::ifstream sourceFile(file.c_str());
  std::string sourceCode(
    std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));

  return ret_val;
}

}// namespace ningine
