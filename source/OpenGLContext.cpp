#include "OpenGLContext.hpp"

#include <iostream>
#include <iomanip>

namespace
{
constexpr char OpenGLShaderProgramName[] = "BasicView";
// TODO: Should be reworked to more advanced approach, withoud hardcoded path
constexpr char VertexShaderPath[] = "../../../source/shaders/GLSL/basic.vert";
constexpr char FragmentShaderPath[] = "../../../source/shaders/GLSL/basic.frag";
}

namespace ningine
{
std::map<int, bool> OpenGLContext::keyMap;

OpenGLContext::OpenGLContext()
  : m_window(nullptr), m_shaderProgram(), m_screenPlane(), windowHeight{}, windowWidth{}
{
}

bool OpenGLContext::init()
{
  if (!createContext())
    return false;

  if (!constructShaders())
    return false;

  m_screenPlane.constructGeometry(&m_shaderProgram, windowWidth, windowHeight);

  initKeyMappings();

  glEnable(GL_DEPTH_TEST);
}

bool OpenGLContext::createContext()
{
  if (!glfwInit())
  {
    std::cerr << "Error, failed to init GLFW: 0x" << std::hex << std::setw(8) << std::setfill('0')
              << glfwGetError(nullptr) << '\n';
    return false;
  }

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

  int numOfMonitors{};
  GLFWmonitor **monitors;
  if ((monitors = glfwGetMonitors(&numOfMonitors)) == nullptr)
  {
    std::cerr << "Error, something went wrong, GLFW haven't found any monitors!\n";
    glfwTerminate();
    return false;
  }
  // GLFWmonitor *monitor = numOfMonitors == 2 ? monitors[1] : monitors[0];
  // For now work only with Default monitor
  GLFWmonitor *monitor = monitors[0];
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

  // window mode without borders
  m_window = glfwCreateWindow(mode->width, mode->height, WindowName, monitor, nullptr);

  windowHeight = mode->height;
  windowWidth = mode->width;

  if (m_window == nullptr)
  {
    std::cerr << "Error, failed to create glfw window!\n";
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(m_window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Error, failed to load the GLAD!\n";
    glfwTerminate();
    return false;
  }

  glfwSetWindowSizeCallback(m_window, resize_callback);
  glfwSetKeyCallback(m_window, key_callback);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  std::cout << "OpenGL version is: " << glGetString(GL_VERSION) << '\n';

  return true;
}

bool OpenGLContext::constructShaders()
{
  if (m_shaderProgram.load(OpenGLShaderProgramName, VertexShaderPath, FragmentShaderPath) == false)
  {
    std::cout << "Failed to load a shader program" << std::endl;
    return false;
  }

  return true;
}

void OpenGLContext::initKeyMappings()
{
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_0, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_1, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_2, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_3, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_4, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_5, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_6, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_7, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_8, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_9, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_DIVIDE, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_MULTIPLY, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_ADD, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_SUBTRACT, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_LEFT_SHIFT, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_RIGHT_SHIFT, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_BACKSPACE, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_DELETE, false));

  keyMap.insert(std::pair<int, bool>(GLFW_KEY_A, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_D, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_W, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_S, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_LEFT_SHIFT, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_SPACE, false));

  keyMap.insert(std::pair<int, bool>(GLFW_KEY_UP, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_DOWN, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_LEFT, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_RIGHT, false));

  keyMap.insert(std::pair<int, bool>(GLFW_KEY_PAGE_DOWN, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_PAGE_UP, false));
}

void OpenGLContext::resize_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void OpenGLContext::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  try
  {
    if (action == GLFW_PRESS)
      keyMap.at(key) = true;
    else if (action == GLFW_RELEASE)
      keyMap.at(key) = false;
  }
  catch (...)
  {
  }
}
}// namespace ningine
