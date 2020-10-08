#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <fstream>
#include <sstream>

#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std::chrono;

typedef unsigned short ushort;

constexpr unsigned short WINDOW_WIDTH{ 1000 };
constexpr unsigned short WINDOW_HEIGHT{ 1000 };

inline std::string read_file(const std::string &filePath)
{
  std::ifstream file(filePath);
  std::string content(std::istreambuf_iterator<char>(file.rdbuf()), std::istreambuf_iterator<char>());
  return content;
}

GLFWwindow *initGLFWwindow(const std::string &windowTitle, const ushort &windowWidth, const ushort &windowHeight);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void processArrowsInput(GLFWwindow *window, const GLint &uniformLocation, GLfloat &texturesMix);

GLuint createShader(GLenum type, const std::string &source);
GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader);

GLuint create2DTexture(const std::string &texturePath,
  const GLenum &wrapS,
  const GLenum &wrapT,
  const GLenum &minFilter,
  const GLenum &maxFilter);

inline void printMat4(const glm::mat4& m) 
{
  for (int i = 0; i < 4; ++i) 
    std::cout << '|' << m[i].x << ' ' << m[i].y << ' ' << m[i].z << ' ' << m[i].w << "|\n";
  std::cout << std::endl;
}

int main(int argc, char *argv[])
{
  GLFWwindow *window = initGLFWwindow("OpenGL HELLO WORLD!", WINDOW_WIDTH, WINDOW_HEIGHT);
  if (window == nullptr)
  {
    std::cout << "Oops, failed to create the GLFW window!\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Oops, failed to load the GLAD!\n";
    return -3;
  }

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glEnable(GL_DEPTH_TEST);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // TODO: FIX THIS AWFULL PATHS
  //        1) investigate boost filesystem
  //        2) probably need to copy shaders by script into folder with build
  unsigned int vertexShader = createShader(GL_VERTEX_SHADER, read_file("../../../src/shaders/vertex.vert"));
  unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, read_file("../../../src/shaders/fragment.frag"));

  unsigned int shaderProgram = createShaderProgram(vertexShader, fragmentShader);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  float vertexData[] = { 
    // position coords  //texture coords
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f
  };
  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
  unsigned indices[] = { 
    0, 1, 3, 
    0, 2, 3 
  };

  // setting up buffers for TRIANGLE
  unsigned int VBO, VAO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // binding vertex buffer and array object for TRIANGLE
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // setting up vertex attributes for current array object (for TRIANGLE)
  const GLsizei stride = (3 + 2) * sizeof(float);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, static_cast<void *>(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  stbi_set_flip_vertically_on_load(true);
  // setting up textures
  unsigned int backTexture =
    create2DTexture("resources/textures/container.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

  unsigned int frontTexture =
    create2DTexture("resources/textures/pepe.png", GL_MIRRORED_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

  glUseProgram(shaderProgram);
  glUniform1i(glGetUniformLocation(shaderProgram, "backTexture"), 0);
  glUniform1i(glGetUniformLocation(shaderProgram, "frontTexture"), 1);

  GLfloat texMixVal = 0.2f;
  GLint texMixLocation = glGetUniformLocation(shaderProgram, "texturesMixCoefficient");

  glm::mat4 trans(1.0f);

  glm::mat4 orthographic = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
  glm::mat4 model(1.0f);
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  glm::mat4 view(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

  glm::mat4 projection = glm::perspective(
    glm::radians(45.0f), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f);

  GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
  GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
  GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

  auto t_start = high_resolution_clock::now();

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);
    processArrowsInput(window, texMixLocation, texMixVal);

    // RENDERING
    auto t_now = high_resolution_clock::now();
    float time = duration_cast<duration<float>>(t_start - t_now).count();

    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, backTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, frontTexture);

    model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.7f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glDeleteProgram(shaderProgram);

  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);

  glfwTerminate();
  return 0;
}

GLFWwindow *initGLFWwindow(const std::string &windowTitle, const ushort &windowWidth, const ushort &windowHeight)
{
  if (glfwInit() == GLFW_FALSE)
  {
    std::cout << "Oops, failed to initializeGLFW!\n";
    return nullptr;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE;
#endif

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), 
		nullptr, nullptr);

  return window;
}

GLuint createShader(GLenum type, const std::string &source)
{
  unsigned int shader = glCreateShader(type);

  const char *source_cstr = source.c_str();

  glShaderSource(shader, 1, &source_cstr, nullptr);
  glCompileShader(shader);

  int success{};
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);

    char *shaderType = (type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
    std::cout << "ERROR! : " << shaderType << " shader compilation failed\n" << infoLog << '\n';

    return 0;
  }

  return shader;
}

GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader)
{
  GLuint shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  int success{};
  char infoLog[512];

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    std::cout << "ERROR! : Shader program linking failed\n" << infoLog << '\n';
    return 0;
  }

  return shaderProgram;
}

GLuint create2DTexture(const std::string &texturePath,
  const GLenum &wrapS,
  const GLenum &wrapT,
  const GLenum &minFilter,
  const GLenum &maxFilter)
{
  GLuint textureDescriptor{};

  glGenTextures(1, &textureDescriptor);
  glBindTexture(GL_TEXTURE_2D, textureDescriptor);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);

  int width{}, height{}, numberOfChannels{};

  unsigned char *data =
    stbi_load(texturePath.c_str(), &width, &height, &numberOfChannels, 0);

  if (data)
  {
    GLenum channels = numberOfChannels == 3 ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, channels, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
    std::cout << "Error, failed to load the texture!\n";

  stbi_image_free(data);

  return textureDescriptor;
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void processArrowsInput(GLFWwindow *window, const GLint &uniformLocation, GLfloat &texturesMix)
{
  constexpr float texturesMixIncome{ 0.0035f };

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && texturesMix < 1.0f)
    glUniform1f(uniformLocation, texturesMix += texturesMixIncome);

  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && texturesMix > 0.0f)
    glUniform1f(uniformLocation, texturesMix -= texturesMixIncome);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}
