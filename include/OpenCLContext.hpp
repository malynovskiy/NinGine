#include "CLWrapper.hpp"
#include "NingineInit.hpp"

namespace ningine
{
class OpenCLContext
{
public:
  OpenCLContext();
  ~OpenCLContext() {}

  int init(GLFWwindow *glWindow, const std::string &kernelPath, uint glTextureID);

private:
  CLWrapper m_clWrapper;

  cl::ImageGL glScreenImage;
};
}
