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
// for test purposes lower reso
// int Ningine::screenWidth = 1366;
// int Ningine::screenHeight = 768;

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

  int numOfMonitors{};
  GLFWmonitor **monitors;
  if ((monitors = glfwGetMonitors(&numOfMonitors)) == nullptr)
  {
    std::cerr << "Error, something went wrong, GLFW haven't found any monitors!\n";
    glfwTerminate();
    return false;
  }
  // GLFWmonitor *monitor = numOfMonitors == 2 ? monitors[1] : monitors[0];
  GLFWmonitor *monitor = monitors[0];
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

  // window mode without borders
  window = glfwCreateWindow(screenWidth, screenHeight, "Realtime ratracing test", monitor, nullptr);

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

  initKeyboardMappings();

  glEnable(GL_DEPTH_TEST);

  camPos = { screenWidth / 2.0f, screenHeight / 2.0f, 0.0f };
  coordinateBasis = camPos;

  createSpheres();
  createTriangles();
  createLighting();

  createScreenImage();

  // pushing our spheres data to the OpenCL buffer
  clContext.createBuffer(CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
    sizeof(float) * spheres.size(),
    spheres.data());

  // pushing our lighting data to the OpenCL buffer
  clContext.createBuffer(CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
    sizeof(float) * lightSources.size(),
    lightSources.data());

  // TODO: Rework for to be customizable
  screenDistance = calculateDist(60);

  curr_coordinate = start_sphere_pos;

  std::cout << "screenDist\t" << screenDistance << "\n";

  std::cout << "FOV:\t"
            << calculateFOV(glm::vec2(screenHeight, 0),
                 glm::vec2(0, screenDistance),
                 glm::vec2(screenWidth, screenDistance));
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

    printFrameRate();
  }

  glfwTerminate();
  return 0;
}

void Ningine::printFrameRate()
{
  static float framesPerSecond = 0.0f;
  static int fps;
  static float lastTime = 0.0f;
  float currentTime = GetTickCount() * 0.001f;
  ++framesPerSecond;
  std::cout << "FPS: " << fps << '\n';
  if (currentTime - lastTime > 1.0f)
  {
    lastTime = currentTime;
    fps = (int)framesPerSecond;
    framesPerSecond = 0;
  }
}

void Ningine::display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program.handle());

  screenPlane.render(textureID);

  clContext.clearBuffers();
  clContext.createBuffer(CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
    sizeof(float) * spheres.size(),
    spheres.data());

  clContext.createBuffer(CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
    sizeof(float) * lightSources.size(),
    lightSources.data());

  clContext.setKernelArgs(0, screenImage);
  clContext.setKernelArgs(1, camPos);
  clContext.setKernelArgs(2, screenDistance);
  clContext.setKernelArgs(3, clContext.getBuffer(0));
  clContext.setKernelArgs(4, numberOfSpheres);
  clContext.setKernelArgs(5, clContext.getBuffer(1));
  clContext.setKernelArgs(6, numberOfLightSources);

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
  processKeyboardInput();
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

  clContext.init(raytracer_kernel_path, contextProperties);

  error = CL_SUCCESS;
  screenImage =
    cl::ImageGL(clContext.getContext(), CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, textureID, &error);

  if (error != CL_SUCCESS)
  {
    std::cerr << "error creating cl::ImageGL:\t" << getErrorString(error) << std::endl;
    return (error);
  }

  createCLKernels();

  return true;
}

float Ningine::calculateFOV(glm::vec2 a, glm::vec2 b, glm::vec2 c)
{
  glm::vec2 v1 = glm::normalize(b - a);
  glm::vec2 v2 = glm::normalize(c - a);
  return (acos(glm::dot(v1, v2)) * 180 / PI);
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
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);

  glUniform1i(glGetUniformLocation(program.handle(), "tex"), 0);
}

void Ningine::pushBackSphere(const Sphere &sphere)
{
  spheres.push_back(sphere.center.x);
  spheres.push_back(sphere.center.y);
  spheres.push_back(sphere.center.z);

  spheres.push_back(sphere.radius);

  spheres.push_back(sphere.material.diffuse_color.x);
  spheres.push_back(sphere.material.diffuse_color.y);
  spheres.push_back(sphere.material.diffuse_color.z);

  spheres.push_back(sphere.material.albedo.x);
  spheres.push_back(sphere.material.albedo.y);
  spheres.push_back(sphere.material.albedo.z);
  spheres.push_back(sphere.material.albedo.w);

  spheres.push_back(sphere.material.specular_exponent);
  spheres.push_back(sphere.material.refractive_index);

  numberOfSpheres++;

  assert(spheres.size() / numberOfSpheres == attrsPerSphere);
}

void Ningine::pushBackLightSource(const LightSource &lightSource)
{
  lightSources.push_back(lightSource.position.x);
  lightSources.push_back(lightSource.position.y);
  lightSources.push_back(lightSource.position.z);

  lightSources.push_back(lightSource.intensity);

  numberOfLightSources++;
  assert(lightSources.size() / numberOfLightSources == attrsPerLightSource);
}

// basic scene with some spheres
void Ningine::createSpheres()
{
  using math::Vec3f;
  numberOfSpheres = 0;

  const Vec3f basis(coordinateBasis.x, coordinateBasis.y, coordinateBasis.z);

  pushBackSphere(Sphere(basis + Vec3f(5, 0, 10 + 40 - 30), 2, materials::ivory));
  pushBackSphere(Sphere(basis + Vec3f(-1.0, -1.5, 12 + 40), 2, materials::glass));
  pushBackSphere(Sphere(basis + Vec3f(1.5, -0.5, 18 + 40), 3, materials::red_rubber));
  pushBackSphere(Sphere(basis + Vec3f(7, 5, 18 + 50), 4, materials::mirror));

  spheres.shrink_to_fit();

  // TODO: Replace this code later to a more robust approach of moving "lead" sphere
  spherePos.x = spheres.at(0);
  spherePos.y = spheres.at(1);
  spherePos.z = spheres.at(2);
}

void Ningine::createTriangles() 
{
  using math::Vec3f;
  //numberOfTriangles = 0;

  const Vec3f basis(coordinateBasis.x, coordinateBasis.y, coordinateBasis.z);
}

void Ningine::createLighting()
{
  numberOfLightSources = 0;

  const math::Vec3f basis(coordinateBasis.x, coordinateBasis.y, coordinateBasis.z);

  // probably need more intensity
  pushBackLightSource(LightSource(basis + math::Vec3f(-20.0f, 30.0f, 20.0f), 3.5f));
  pushBackLightSource(LightSource(basis + math::Vec3f(20.0f, 20.0f, 30.0f), 1.5f));
  lightSources.shrink_to_fit();
}

void Ningine::initKeyboardMappings()
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

void Ningine::processKeyboardInput()
{
  // first sphere movement
  if (keyMap.at(GLFW_KEY_LEFT))
    spherePos.x -= 0.1f;

  if (keyMap.at(GLFW_KEY_RIGHT))
    spherePos.x += 0.1f;

  if (keyMap.at(GLFW_KEY_UP))
    spherePos.z += 0.1f;

  if (keyMap.at(GLFW_KEY_DOWN))
    spherePos.z -= 0.1f;

  if (keyMap.at(GLFW_KEY_PAGE_DOWN))
    spherePos.y -= 0.1f;

  if (keyMap.at(GLFW_KEY_PAGE_UP))
    spherePos.y += 0.1f;


  if (keyMap.at(GLFW_KEY_W))
    camPos.z += 0.5f;

  if (keyMap.at(GLFW_KEY_S))
    camPos.z -= 0.5f;

  if (keyMap.at(GLFW_KEY_A))
    camPos.x -= 0.3f;

  if (keyMap.at(GLFW_KEY_D))
    camPos.x += 0.3f;

  if (keyMap.at(GLFW_KEY_LEFT_SHIFT))
    camPos.y -= 0.3f;

  if (keyMap.at(GLFW_KEY_SPACE))
    camPos.y += 0.3f;

  // if (keyMap.at(GLFW_KEY_BACKSPACE))
  //{
  //  if (numberOfSpheres > 0)
  //  {
  //    spheres.pop_back();
  //    numberOfSpheres--;
  //    curr_coordinate.x -= spheres_offset.x;
  //  }
  //  if (!(keyMap.at(GLFW_KEY_LEFT_SHIFT) || keyMap.at(GLFW_KEY_RIGHT_SHIFT)))
  //  {
  //    keyMap.at(GLFW_KEY_BACKSPACE) = false;
  //  }
  //}

  // if (keyMap.at(GLFW_KEY_DELETE))
  //{
  //  if (numberOfSpheres > 0)
  //  {
  //    spheres.clear();
  //    numberOfSpheres = 0;
  //    curr_coordinate = start_sphere_pos;
  //  }
  //}
  // if (keyMap.at(GLFW_KEY_KP_7))
  //{
  //  keyMap.at(GLFW_KEY_KP_7) = false;
  //  for (int i = 0; i < 100; i++)
  //  {
  //    addSphere(glm::vec3(curr_coordinate.x, curr_coordinate.y, curr_coordinate.z),
  //      5,
  //      glm::vec3(randDistribution(randGen), randDistribution(randGen),
  //      randDistribution(randGen)), glm::vec3(0.8, 0.8, 0.8), glm::vec3(0.9, 0.9, 0.9),
  //      glm::vec3(0.2, 0.2, 0.2),
  //      glm::vec3(0.8, 0.8, 0.8),
  //      glm::vec3(0.9, 0.9, 0.9),
  //      50,
  //      0.0f,
  //      1.0f,
  //      1.0f);
  //    curr_coordinate.x += spheres_offset.x;
  //    if (curr_coordinate.x > ((screenWidth / 2.0f) + 30))
  //    {
  //      curr_coordinate.x = (screenWidth / 2.0f) - 33;
  //      curr_coordinate.y += spheres_offset.y;
  //      if (curr_coordinate.y > ((screenHeight / 2.0f) + 15))
  //      {
  //        curr_coordinate.y = (screenHeight / 2.0f) - 15;
  //        curr_coordinate.z += spheres_offset.z;
  //      }
  //    }
  //  }
  //}
  // if (curr_coordinate.x > ((screenWidth / 2.0f) + 30))
  //{
  //  curr_coordinate.x = (screenWidth / 2.0f) - 33;
  //  curr_coordinate.y += spheres_offset.y;
  //  if (curr_coordinate.y > ((screenHeight / 2.0f) + 15))
  //  {
  //    curr_coordinate.y = (screenHeight / 2.0f) - 15;
  //    curr_coordinate.z += spheres_offset.z;
  //  }
  //}
  // if (curr_coordinate.x < ((screenWidth / 2.0f) - 33))
  //{
  //  curr_coordinate.x = (screenWidth / 2.0f) + 27;
  //  curr_coordinate.y -= spheres_offset.y;
  //  if (curr_coordinate.y < ((screenHeight / 2.0f) - 15))
  //  {
  //    curr_coordinate.y = (screenHeight / 2.0f) + 15;
  //    curr_coordinate.z -= spheres_offset.z;
  //  }
  //}

  // TODO: Output things on the window
  // std::cout << "Number of spheres\t" << numberOfSpheres << "\n";
  if (!spheres.empty())
  {
    spheres.at((0 * attrsPerSphere) + 0) = spherePos.x;
    spheres.at((0 * attrsPerSphere) + 1) = spherePos.y;
    spheres.at((0 * attrsPerSphere) + 2) = spherePos.z;

    /*std::cout << "Sphere pos: (" << spherePos.x << ", " << spherePos.y << ", " << spherePos.z
              << "\n";*/
  }
}

}// namespace ningine
