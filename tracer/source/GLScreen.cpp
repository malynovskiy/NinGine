#include "GLScreen.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace ningine
{
int GLScreen::numOfVertices = 8;
int GLScreen::numOfTriangleIndices = 12;

GLScreen::GLScreen()
{
  resetMatrix();
}

void GLScreen::render(GLuint texID)
{
  if (program == nullptr)
  {
    std::cerr << "Error: shader nullptr" << std::endl;
    return;
  }

  glUniformMatrix4fv(glGetUniformLocation(program->handle(), "ModelViewMatrix"),
    1,
    GL_FALSE,
    glm::value_ptr(matrix));

  // start rendering
  glBindVertexArray(m_vaoID);// select VAO
  glActiveTexture(GL_TEXTURE0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
  glDrawElements(GL_TRIANGLES, numOfTriangleIndices * 3, GL_UNSIGNED_INT, 0);
  // end rendering

  glBindVertexArray(0);
}

void GLScreen::constructGeometry(
  GLShaderProgram *shaderProgram, const uint width, const uint height)
{
  setShaderProgram(shaderProgram);
  float a{}, b{};

  a = std::max(width, height);
  b = a == width ? height : width;

  const float ratio = b / a;

  vertices = { 
  // --X-- --Y--  --Z--
     1.0f, -1.0f, -1.0f, 
     1.0f,  1.0f, -1.0f, 
    -1.0f,  1.0f, -1.0f, 
    -1.0f, -1.0f, -1.0f 
  };

  colors = { 
  //--R-- --G-- --B--
    1.0f, 1.0f, 1.0f, 
    1.0f, 1.0f, 1.0f, 
    1.0f, 1.0f, 1.0f, 
    1.0f, 1.0f, 1.0f 
  };

  textureCoords = vecf(8, 0.0f);
  if (width > height)
  {
    textureCoords[0] = 1.0f;
    textureCoords[2] = 1.0f;
    textureCoords[3] = ratio;
    textureCoords[5] = ratio;
  }
  else
  {
    textureCoords[0] = ratio;
    textureCoords[2] = ratio;
    textureCoords[3] = 1.0f;
    textureCoords[5] = 1.0f;
  }

  trIndices = { 
    0, 1, 2, 
    0, 2, 3 
  };

  glGenVertexArrays(1, &m_vaoID);
  glBindVertexArray(m_vaoID);

  glGenBuffers(3, m_vboID);

  GLuint spHandle = shaderProgram->handle();
  GLint uniformLocation{};

  // VERTEX POSITIONS DATA
  glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
  glBufferData(
    GL_ARRAY_BUFFER, numOfVertices * 3 * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
  uniformLocation = glGetAttribLocation(spHandle, "in_Position");
  glVertexAttribPointer(uniformLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(uniformLocation);

  // COLORS DATA
  glBindBuffer(GL_ARRAY_BUFFER, m_vboID[1]);
  glBufferData(GL_ARRAY_BUFFER, numOfVertices * 3 * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
  uniformLocation = glGetAttribLocation(spHandle, "in_Color");
  glVertexAttribPointer(uniformLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(uniformLocation);

  // TEXTURE COORDS DATA
  glBindBuffer(GL_ARRAY_BUFFER, m_vboID[2]);
  glBufferData(
    GL_ARRAY_BUFFER, numOfVertices * 2 * sizeof(GLfloat), textureCoords.data(), GL_STATIC_DRAW);
  uniformLocation = glGetAttribLocation(spHandle, "in_TexCoord");
  glVertexAttribPointer(uniformLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(uniformLocation);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // INDICES
  glGenBuffers(1, &m_iboID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
    numOfTriangleIndices * 3 * sizeof(unsigned int),
    trIndices.data(),
    GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
}
}// namespace ningine
