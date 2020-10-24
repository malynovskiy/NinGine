#include "graphics/Shader.h"
#include <fstream>
#include <sstream>
namespace ningine {
namespace graphics {

  inline std::string readFromFile(const char *path)
  {
    std::ifstream file;
    file.open(path);

    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

    return stream.str();
  }

  ShaderProgram::ShaderProgram(const char *vertexShaderPath, const char *fragmentShaderPath)
  {
    std::string vsSource = readFromFile(vertexShaderPath);
    std::string fsSource = readFromFile(fragmentShaderPath);
  }
  void ShaderProgram::use()
  {
  }
}// namespace graphics
}// namespace ningine
