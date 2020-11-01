#include "GLProgram.h"
#include <fstream>
#include <iostream>

namespace ningine
{

inline std::string readfromFile(const std::string &filePath)
{
  std::ifstream file(filePath);
  std::string content(std::istreambuf_iterator<char>(file.rdbuf()), std::istreambuf_iterator<char>());
  return content;
}

GLProgram::GLProgram() : m_name(""), m_vertexShader(0), m_fragmentShader(0), m_program(0) {}

GLProgram::~GLProgram()
{
  glUseProgram(0);
  glDeleteShader(m_vertexShader);
  glDeleteShader(m_fragmentShader);
  glDeleteProgram(m_program);
}

bool GLProgram::load(const std::string name, std::string vertexFileName, std::string fragmentFileName)
{
  m_name = name;
  GLint success = 0;

  m_vertexShader = createShader(vertexFileName, GL_VERTEX_SHADER, success);
  if (!success)
    return false;

  m_fragmentShader = createShader(fragmentFileName, GL_FRAGMENT_SHADER, success);
  if (!success)
    return false;

  m_program = glCreateProgram();

  glAttachShader(m_program, m_vertexShader);
  glAttachShader(m_program, m_fragmentShader);

  glLinkProgram(m_program);

  glGetProgramiv(m_program, GL_LINK_STATUS, &success);
  if (!success)
  {
    char infoLog[512];
    glGetProgramInfoLog(m_program, 512, nullptr, infoLog);

    std::cerr << std::endl << "Error linking GLSL shaders into a shader program." << std::endl;
    std::cerr << "GLSL vertex shader: '" << vertexFileName << "'" << std::endl;
    std::cerr << "GLSL fragment shader: '" << fragmentFileName << "'" << std::endl << std::endl;
    std::cerr << "Program info log:" << std::endl << infoLog << std::endl;

    return false;
  }

  std::cout << "Loaded GLSL program: '" << m_name << "'" << std::endl;

  return true;
}

GLuint GLProgram::createShader(std::string fileName, GLenum type, GLint &success)
{
  const char *source_cstr = readfromFile(fileName).c_str();

  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &source_cstr, nullptr);
  glCompileShader(shader);

  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    char *shaderType = (type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
    std::cerr << std::endl
              << "Error compiling GLSL " << shaderType << " shader: '" << fileName << "'" << std::endl
              << std::endl;
    std::cerr << "Shader info log:" << std::endl << infoLog << std::endl;

    return -1;
  }

  return shader;
}

std::string GLProgram::shaderInfoLog(const GLuint shader) const
{
  GLsizei infoLogSize = 0;
  std::string infoLog;

  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogSize);
  infoLog.resize(infoLogSize);
  glGetShaderInfoLog(shader, infoLogSize, &infoLogSize, &infoLog[0]);

  return infoLog;
}


std::string GLProgram::programInfoLog(const GLuint program) const
{
  GLsizei infoLogSize = 0;
  std::string infoLog;

  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogSize);
  infoLog.resize(infoLogSize);
  glGetProgramInfoLog(program, infoLogSize, &infoLogSize, &infoLog[0]);

  return infoLog;
}

}// namespace ningine
