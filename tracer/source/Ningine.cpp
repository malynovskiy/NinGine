#include "Ningine.hpp"

#include <iostream>
#include <iomanip>
#include <cassert>
#include "OpenCLUtility.hpp"

#define PI 3.14159265358979

namespace ningine
{

int Ningine::screenWidth = 1920;
int Ningine::screenHeight = 1080;

std::map<int, bool> Ningine::keyMap;

bool Ningine::createGLContext()
{
  if (!glfwInit())
  {
    std::cerr << "Error, failed to init glfw: 0x" << std::hex << std::setw(8) << std::setfill('0')
              << glfwGetError(nullptr) << '\n';
    return false;
  }

  // TODO: verify do we need the next hint at all?
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  // TODO: probably the next hint will be useful for fps capabilities
  glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

  window = glfwCreateWindow(screenWidth, screenHeight, "Real-Time Ray-Tracing test", glfwGetPrimaryMonitor(), nullptr);
  if (window == nullptr)
  {
    std::cerr << "Error, failed to create glfw window!\n";
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Error, failed to load the GLAD!\n";
    glfwTerminate;
    return false;
  }

  std::cout << "OpenGL version is: " << glGetString(GL_VERSION) << '\n';

  return true;
}

bool Ningine::init()
{
  if (!createGLContext())
    return false;

  randDistribution = std::uniform_real_distribution<float>(0.0f, 1.0f);

  glfwSetWindowSizeCallback(window, resize_callback);
  glfwSetKeyCallback(window, key_callback);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  if (!program.load(glShaderProgramName, vertexShaderName, fragmentShaderName))
    std::cerr << "Failed to load a shader program" << std::endl;

  screenPlane.constructGeometry(&program, screenWidth, screenHeight);

  initKeyMappings();

  glEnable(GL_DEPTH_TEST);

  createSpheres();
  createScreenImage();

  // pushing our spheres data to the common OpenCL buffer
  clContext.createBuffer(
    CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * spheres.size(), spheres.data());

  // mock ligting for now
  float lighting[6] = { 0.8, 0.8, 0.8, 0.9, 0.9, 0.9 };
  clContext.createBuffer(CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * 6, lighting);

  // TODO: Rework for to be customizable
  screenDistance = calculateDist(60);

  camPos = { screenWidth / 2.0f, screenHeight / 2.0f, 0.0f };

  // main sphere with movement
  spherePos = glm::vec3((screenWidth / 2.0) - 33, (screenHeight / 2.0) - 15, 70);


  std::cout << "screenDist\t" << screenDistance << "\n";

  std::cout << "FOV:\t"
            << calculateFOV(glm::vec2(640, 0), glm::vec2(0, screenDistance), glm::vec2(1280, screenDistance));
  std::cout << (char)167 << "\n";

  glfwSwapInterval(1);
  resize_callback(window, screenWidth, screenHeight);
  return true;
}

int Ningine::run()
{
  while (!glfwWindowShouldClose(window))
  {
    display();
    glfwSwapBuffers(window);
    processEvents();
  }

  glfwTerminate();
  return 0;
}

void Ningine::display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program.handle());

  screenPlane.render(textureID);

  clContext.clearBuffers();
  clContext.createBuffer(
    CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * spheres.size(), spheres.data());

  // temporal lighting
  float lighting[6] = { 0.8, 0.8, 0.8, 0.9, 0.9, 0.9 };
  clContext.createBuffer(
    CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * 6, &lighting[0]);

  clContext.setKernelArgs(0, screenImage);
  clContext.setKernelArgs(1, camPos);
  clContext.setKernelArgs(2, screenDistance);
  clContext.setKernelArgs(3, clContext.getBuffer(0));
  clContext.setKernelArgs(4, numberOfSpheres);
  clContext.setKernelArgs(5, clContext.getBuffer(1));

  screenRange = cl::NDRange(screenWidth, screenHeight);

  queue = cl::CommandQueue(clContext.getContext(), clContext.getDevice());
  std::vector<cl::Memory> images(1, screenImage);

  // tell openGL to let openCL use the memory
  queue.enqueueAcquireGLObjects(&images, nullptr);
  queue.enqueueNDRangeKernel(clContext.getKernel(), 0, screenRange);
  // give the memory back to openGL
  queue.enqueueReleaseGLObjects(&images, nullptr);

  glUseProgram(0);// turn off the current shader
}

void Ningine::processEvents()
{
  glfwPollEvents();
}

void Ningine::createScreenImage()
{
  initGLTexture();
  initCLContext();
}

bool Ningine::initCLContext()
{
  cl_int error = CL_SUCCESS;

  cl::Platform lPlatform = getPlatform();
  cl_context_properties contextProperties[] = { CL_GL_CONTEXT_KHR,
    (cl_context_properties)glfwGetWGLContext(window),
    CL_WGL_HDC_KHR,
    (cl_context_properties)GetDC(glfwGetWin32Window(window)),
    CL_CONTEXT_PLATFORM,
    (cl_context_properties)lPlatform(),
    0 };

  clContext.init("CLfiles/raytracer.cl", contextProperties);

  error = CL_SUCCESS;
  screenImage = cl::ImageGL(clContext.getContext(), CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, textureID, &error);

  if (error != CL_SUCCESS)
  {
    std::cerr << "error creating cl::ImageGL:\t" << getErrorString(error) << std::endl;
    return (error);
  }

  createCLKernels();

  return true;
}

float Ningine::calculateDist(float fov)
{
  return (screenWidth / 2.0f) / tan(fov / 2.0 * PI / 180.0f);
}

void Ningine::createCLKernels()
{
  clContext.createKernel("raytracer");
}

void Ningine::initGLTexture()
{
  // allocate the space for the window texture
  int screenDim = ((screenWidth > screenHeight) ? screenWidth : screenHeight);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenDim, screenDim, 0, GL_RGB, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

  glUniform1i(glGetUniformLocation(program.handle(), "tex"), 0);
}

// Pushes sphere data into the common buffer the will be passed into OpenCL kernels
void Ningine::addSphere(glm::vec3 position,
  float radius,
  glm::vec3 color,
  glm::vec3 lightAmbiant,
  glm::vec3 lightSpecular,
  glm::vec3 materialAmbiant,
  glm::vec3 materialDiffuse,
  glm::vec3 materialSpecular,
  float materialShinyness,
  float reflectiveIndex,
  float opacity,
  float refractiveIndex)
{
  spheres.push_back(position.x);
  spheres.push_back(position.y);
  spheres.push_back(position.z);

  spheres.push_back(radius);

  spheres.push_back(color.r);
  spheres.push_back(color.g);
  spheres.push_back(color.b);

  spheres.push_back(materialAmbiant.r);
  spheres.push_back(materialAmbiant.g);
  spheres.push_back(materialAmbiant.b);

  spheres.push_back(materialDiffuse.r);
  spheres.push_back(materialDiffuse.g);
  spheres.push_back(materialDiffuse.b);

  spheres.push_back(materialSpecular.r);
  spheres.push_back(materialSpecular.g);
  spheres.push_back(materialSpecular.b);

  spheres.push_back(materialShinyness);

  spheres.push_back(reflectiveIndex);

  spheres.push_back(opacity);

  spheres.push_back(refractiveIndex);

  numberOfSpheres++;

  assert(spheres.size() / numberOfSpheres == attrsPerSphere);
}

// basic scene with some spheres
void Ningine::createSpheres()
{
  addSphere(glm::vec3(620, 360, 70),
    15,
    glm::vec3(1, 1, 1),
    glm::vec3(0.8, 0.8, 0.8),
    glm::vec3(0.9, 0.9, 0.9),
    glm::vec3(0.2, 0.2, 0.2),
    glm::vec3(0.8, 0.8, 0.8),
    glm::vec3(0.9, 0.9, 0.9),
    50,
    0.50f,
    0.3f,
    0.8f);

  addSphere(glm::vec3(668, 341, 100),
    15,
    glm::vec3(1, 0, 0),
    glm::vec3(0.8, 0.8, 0.8),
    glm::vec3(0.9, 0.9, 0.9),
    glm::vec3(0.2, 0.2, 0.2),
    glm::vec3(0.8, 0.8, 0.8),
    glm::vec3(0.9, 0.9, 0.9),
    50,
    0.90f,
    1.0f,
    0.7f);
  addSphere(glm::vec3(632, 374, 110),
    5,
    glm::vec3(0, 1, 0),
    glm::vec3(0.8, 0.8, 0.8),
    glm::vec3(0.9, 0.9, 0.9),
    glm::vec3(0.2, 0.2, 0.2),
    glm::vec3(0.8, 0.8, 0.8),
    glm::vec3(0.9, 0.9, 0.9),
    50,
    0.90f,
    1.0f,
    0.7f);
  addSphere(glm::vec3(646, 354, 107),
    7,
    glm::vec3(0, 0, 1),
    glm::vec3(0.8, 0.8, 0.8),
    glm::vec3(0.9, 0.9, 0.9),
    glm::vec3(0.2, 0.2, 0.2),
    glm::vec3(0.8, 0.8, 0.8),
    glm::vec3(0.9, 0.9, 0.9),
    50,
    0.90f,
    1.0f,
    0.7f);

  addSphere(glm::vec3(618, 348, 100),
    5,
    glm::vec3(1, 1, 0),
    glm::vec3(0.8, 0.8, 0.8),
    glm::vec3(0.9, 0.9, 0.9),
    glm::vec3(0.2, 0.2, 0.2),
    glm::vec3(0.8, 0.8, 0.8),
    glm::vec3(0.9, 0.9, 0.9),
    50,
    0.90f,
    1.0f,
    0.7f);
  addSphere(glm::vec3(602, 369, 115),
    10,
    glm::vec3(0, 1, 1),
    glm::vec3(0.8, 0.8, 0.8),
    glm::vec3(0.9, 0.9, 0.9),
    glm::vec3(0.2, 0.2, 0.2),
    glm::vec3(0.8, 0.8, 0.8),
    glm::vec3(0.9, 0.9, 0.9),
    50,
    0.90f,
    1.0f,
    0.7f);
  addSphere(glm::vec3(663, 371, 120),
    20,
    glm::vec3(1, 0, 1),
    glm::vec3(0.8, 0.8, 0.8),
    glm::vec3(0.9, 0.9, 0.9),
    glm::vec3(0.2, 0.2, 0.2),
    glm::vec3(0.8, 0.8, 0.8),
    glm::vec3(0.9, 0.9, 0.9),
    50,
    0.90f,
    1.0f,
    0.7f);

  spheres.shrink_to_fit();
}

void Ningine::initKeyMappings()
{
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_1, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_2, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_3, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_4, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_5, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_6, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_7, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_8, false));
  keyMap.insert(std::pair<int, bool>(GLFW_KEY_KP_9, false));
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

void Ningine::resize_callback(GLFWwindow *window, int width, int height)
{
  screenWidth = width;
  screenHeight = height;

  glViewport(0, 0, width, height);
}

void Ningine::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
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
