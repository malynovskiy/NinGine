#include "Ningine.hpp"

#include "OpenCLUtility.hpp"
#include "OpenCLContextWrapper.hpp"
#include "OpenGLContextWrapper.hpp"
#include "Utility.hpp"

#include <iostream>
#include <iomanip>
#include <cassert>

namespace ningine
{
bool Ningine::init()
{
  if (!m_glContext.init())
    return false;

  const uint glTextureID = m_glContext.initTexture();
  //randDistribution = std::uniform_real_distribution<float>(0.0f, 1.0f);

  m_clContext.init(m_glContext.getWindowHandle(), raytracer_kernel_path, glTextureID);
  
  const uint2 windowSize = m_glContext.getWindowSize();
  const float3 windowCenter{ windowSize.x / 2.0f, windowSize.y / 2.0f, 0.0f };
  m_scene.create(windowCenter, windowCenter, windowSize);

  // creating OpenCL bufferes with objects and lights data
  m_clContext.createBuffer(m_scene.getObjects());
  m_clContext.createBuffer(m_scene.getLights());

  // TODO: Rework for to be customizable
  //curr_coordinate = start_sphere_pos;

  return true;
}

int Ningine::run()
{
  while (!m_glContext.shouldCloseWindow())
  {
    render();

    processEvents();

    spheresMove();
    
    printFrameRate();
  }

  return 0;
}

void Ningine::printFrameRate()
{
  static float framesPerSecond = 0.0f;
  static int fps;
  static float lastTime = 0.0f;
  float currentTime = 0.0f;

  #ifdef WINDOWS
  currentTime = GetTickCount64() * 0.001f;
  #endif
  // TODO: Should be addressed Linux and Mac there -> GetTickCount alternative

  ++framesPerSecond;
  std::cout << "FPS: " << fps << '\n';
  if (currentTime - lastTime > 1.0f)
  {
    lastTime = currentTime;
    fps = (int)framesPerSecond;
    framesPerSecond = 0;
  }
}

void Ningine::render()
{
  m_glContext.render();
  utility::ScopeGuard glRenderingFinilize([&]() { m_glContext.post_render(); });

  // TODO: Investigate do we need to clear every time buffers and create them again
  //        maybe we could optimize this place by reusing existing buffers without clearing?
  m_clContext.clearBuffers();
  m_clContext.createBuffer(m_scene.getObjects());
  m_clContext.createBuffer(m_scene.getLights());

  uint kernelArgsIndex{};
  m_clContext.setKernelArgs(kernelArgsIndex++, m_clContext.getScreenImage());
  m_clContext.setKernelArgs(kernelArgsIndex++, m_scene.getCameraPosition());
  m_clContext.setKernelArgs(kernelArgsIndex++, m_scene.getScreenDistance());
  m_clContext.setKernelArgs(kernelArgsIndex++, m_clContext.getBuffer(0));
  m_clContext.setKernelArgs(kernelArgsIndex++, m_scene.getNumberOfSpheres());
  m_clContext.setKernelArgs(kernelArgsIndex++, m_clContext.getBuffer(1));
  m_clContext.setKernelArgs(kernelArgsIndex++, m_scene.getNumberOfLights());

  const uint2 windowSize = m_glContext.getWindowSize();
  m_clContext.processKernel({windowSize.x, windowSize.y});
}

void Ningine::processEvents()
{
  m_glContext.pollEvents();
  processKeyboardInput();
}

void Ningine::processKeyboardInput()
{
  // first sphere movement
  /*if (keyMap.at(GLFW_KEY_LEFT))
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
    camPos.y += 0.3f;*/

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
  //if (!spheres.empty())
  //{
  //  spheres.at((0 * attrsPerSphere) + 0) = spherePos.x;
  //  spheres.at((0 * attrsPerSphere) + 1) = spherePos.y;
  //  spheres.at((0 * attrsPerSphere) + 2) = spherePos.z;

  //  /*std::cout << "Sphere pos: (" << spherePos.x << ", " << spherePos.y << ", " << spherePos.z
  //            << "\n";*/
  //}
}

void Ningine::spheresMove() 
{
//  if (!spheres.empty())
//  {
//    float t = 0.001f;
//#ifdef WINDOWS
//    t *= GetTickCount();
//#endif
//    // TODO: Should be addressed Linux and Mac there -> GetTickCount alternative
//    const float sint = sin(t); 
//    spheres.at((3 * attrsPerSphere) + 2) += sint;
//    spheres.at((4 * attrsPerSphere) + 2) -= 1- sint;
//    spheres.at((5 * attrsPerSphere) + 2) += 1- sint;
//    std::cout << sint << std::endl;
//  }
}

}// namespace ningine
