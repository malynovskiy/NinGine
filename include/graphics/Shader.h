#pragma once
#include <string>

namespace ningine {
namespace graphics {
  class ShaderProgram
  {
  public:
    unsigned vertexShaderID;
    unsigned fragmentShaderID;

    ShaderProgram(const char *vertexShaderPath, const char *fragmentShaderPath);
    ~ShaderProgram() {}

    void use();
  };
}// namespace graphics
}// namespace ningine
