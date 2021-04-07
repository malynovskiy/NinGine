#pragma once
#include <CL/cl.hpp>

namespace ningine
{
class GraphicsContextBridge
{
public:
  GraphicsContextBridge() = default;
  ~GraphicsContextBridge() = default;

private:
  cl::NDRange windowSizeRange;
  cl::CommandQueue commandQueue;
  cl::ImageGL glScreenImage;
};
}// namespace ningine
