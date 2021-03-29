#include "OpenGLContextWrapper.hpp"

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
typedef std::pair<int, bool> KeyPair;

std::map<int, bool> OpenGLContextWrapper::keyMap;

OpenGLContextWrapper::OpenGLContextWrapper()
  : m_window(nullptr), m_shaderProgram(), m_screenPlane(), windowSize(DefaultWindowSize), textureID(0)
{
}

bool OpenGLContextWrapper::init()
{
  if (!createContext())
  {
    glfwTerminate();
    return false;
  }

  if (!constructShaders())
    return false;

  m_screenPlane.constructGeometry(&m_shaderProgram, windowSize.x, windowSize.y);

  initKeyMappings();

  glEnable(GL_DEPTH_TEST);
  
  return true;
}

bool OpenGLContextWrapper::createContext()
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

  windowSize = { static_cast<uint>(mode->width), static_cast<uint>(mode->height) };

  if (m_window == nullptr)
  {
    std::cerr << "Error, failed to create glfw window!\n";
    return false;
  }

  glfwMakeContextCurrent(m_window);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
  {
    std::cerr << "Error, failed to load the GLAD!\n";
    return false;
  }

  glfwSetWindowSizeCallback(m_window, resize_callback);
  glfwSetKeyCallback(m_window, key_callback);
  glfwSwapInterval(1);
  resize_callback(m_window, windowSize.x, windowSize.y);
  
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  std::cout << "OpenGL version is: " << glGetString(GL_VERSION) << '\n';

  return true;
}

bool OpenGLContextWrapper::constructShaders()
{
  if (m_shaderProgram.load(OpenGLShaderProgramName, VertexShaderPath, FragmentShaderPath) == false)
  {
    std::cout << "Failed to load a shader program" << std::endl;
    return false;
  }

  return true;
}

uint OpenGLContextWrapper::initTexture()
{
  // allocate the space for the window texture
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  const int screenDim = std::max(windowSize.x, windowSize.y);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenDim, screenDim, 0, GL_RGB, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);

  glUniform1i(glGetUniformLocation(m_shaderProgram.handle(), "tex"), 0);
  
  return textureID;
}

void OpenGLContextWrapper::initKeyMappings()
{
  keyMap.insert(KeyPair(GLFW_KEY_KP_0, false));
  keyMap.insert(KeyPair(GLFW_KEY_KP_1, false));
  keyMap.insert(KeyPair(GLFW_KEY_KP_2, false));
  keyMap.insert(KeyPair(GLFW_KEY_KP_3, false));
  keyMap.insert(KeyPair(GLFW_KEY_KP_4, false));
  keyMap.insert(KeyPair(GLFW_KEY_KP_5, false));
  keyMap.insert(KeyPair(GLFW_KEY_KP_6, false));
  keyMap.insert(KeyPair(GLFW_KEY_KP_7, false));
  keyMap.insert(KeyPair(GLFW_KEY_KP_8, false));
  keyMap.insert(KeyPair(GLFW_KEY_KP_9, false));
  keyMap.insert(KeyPair(GLFW_KEY_KP_DIVIDE, false));
  keyMap.insert(KeyPair(GLFW_KEY_KP_MULTIPLY, false));
  keyMap.insert(KeyPair(GLFW_KEY_KP_ADD, false));
  keyMap.insert(KeyPair(GLFW_KEY_KP_SUBTRACT, false));
  keyMap.insert(KeyPair(GLFW_KEY_LEFT_SHIFT, false));
  keyMap.insert(KeyPair(GLFW_KEY_RIGHT_SHIFT, false));
  keyMap.insert(KeyPair(GLFW_KEY_BACKSPACE, false));
  keyMap.insert(KeyPair(GLFW_KEY_DELETE, false));

  keyMap.insert(KeyPair(GLFW_KEY_A, false));
  keyMap.insert(KeyPair(GLFW_KEY_D, false));
  keyMap.insert(KeyPair(GLFW_KEY_W, false));
  keyMap.insert(KeyPair(GLFW_KEY_S, false));
  keyMap.insert(KeyPair(GLFW_KEY_LEFT_SHIFT, false));
  keyMap.insert(KeyPair(GLFW_KEY_SPACE, false));

  keyMap.insert(KeyPair(GLFW_KEY_UP, false));
  keyMap.insert(KeyPair(GLFW_KEY_DOWN, false));
  keyMap.insert(KeyPair(GLFW_KEY_LEFT, false));
  keyMap.insert(KeyPair(GLFW_KEY_RIGHT, false));

  keyMap.insert(KeyPair(GLFW_KEY_PAGE_DOWN, false));
  keyMap.insert(KeyPair(GLFW_KEY_PAGE_UP, false));
}

// TODO: investigate whether we need resize callback at all
void OpenGLContextWrapper::resize_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void OpenGLContextWrapper::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
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

intptr_t OpenGLContextWrapper::getPlatformGLNativeContext(GLFWwindow *window)
{
  intptr_t nativeContext{};

#if defined(WINDOWS)
  nativeContext = reinterpret_cast<intptr_t>(glfwGetWGLContext(window));
#elif defined(LINUX)
  nativeContext = reinterpret_cast<intptr_t>(glfwGetGLXContext(window));
#elif defined(MAC)
  nativeContext = reinterpret_cast<intptr_t>(glfwGetNSGLContext(window));
#else
  nativeContext = 0;
  assert(false);
#endif

  return nativeContext;
}

intptr_t OpenGLContextWrapper::getPlatformGLNativeWindow(GLFWwindow *window)
{
  intptr_t nativeWindow{};

#if defined(WINDOWS)
  nativeWindow = reinterpret_cast<intptr_t>(GetDC(glfwGetWin32Window(window)));
#elif defined(LINUX)
  nativeWindow = static_cast<intptr_t>(glfwGetGLXWindow(window));
#elif defined(MAC)
  nativeWindow = static_cast<intptr_t>(glfwGetCocoa(window));
#else
  nativeWindow = 0;
  assert(false);
#endif

  return nativeWindow;
}

}// namespace ningine
