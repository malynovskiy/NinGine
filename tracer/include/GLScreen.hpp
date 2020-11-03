#pragma once

#include "GLShaderProgram.hpp"
#include <glm/glm.hpp>
#include <vector>

typedef std::vector<float> vecf;
typedef unsigned int uint;

namespace ningine
{

constexpr int NumberOfVertexCoords = 12;
constexpr int NumberOfTriangleIndices = 6;

class GLScreen
{
  GLuint m_vaoID;   // Vertex Array Object
  GLuint m_vboID[3];// Vertex Buffer Object	-> vertex, color, texture data
  GLuint m_iboID;   // Index Buffer Object

  static int numOfVertices;
  static int numOfTriangleIndices;

  vecf vertices;
  vecf colors;
  vecf textureCoords;
  std::vector<uint> trIndices;// triangle indices

  glm::mat4 matrix;

  GLShaderProgram *program;

public:
  GLScreen();

  inline void setShaderProgram(GLShaderProgram *shaderProgram) { program = shaderProgram; }

  inline void resetMatrix() { matrix = glm::mat4(1.0f); }
  inline void setMatrix(glm::mat4 mat) { matrix = mat; }

  void constructGeometry(GLShaderProgram *shaderProgram, const uint width, const uint height);
  void render(GLuint texID);
};

}// namespace ningine
