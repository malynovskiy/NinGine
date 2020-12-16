#pragma once

#include <string>
#include <glad/glad.h>

namespace ningine
{
class GLShaderProgram
{
public:
  GLShaderProgram();
  ~GLShaderProgram();

  GLShaderProgram(const GLShaderProgram &) = delete;
  GLShaderProgram &operator=(const GLShaderProgram &) = delete;

  GLuint handle() const { return m_program; }
  bool load(const std::string name, std::string vertexFileName, std::string fragmentFileName);

private:
  GLuint GLShaderProgram::createShader(std::string fileName, GLenum type, GLint &success);

  std::string shaderInfoLog(const GLuint shader) const;
  std::string programInfoLog(const GLuint program) const;

  std::string m_name;
  GLuint m_vertexShader;
  GLuint m_fragmentShader;
  GLuint m_program;
};

}// namespace ningine
