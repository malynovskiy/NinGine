#include "Ningine.hpp"

int main(int argc, char *argv[])
{
  ningine::Ningine engine;

  if (!engine.init())
    return -1;

  return engine.run();
}
