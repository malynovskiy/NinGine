#pragma once
#include <functional>

namespace ningine
{
namespace utility
{
  constexpr uint64 kB = 1024;
  constexpr uint64 MB = 1048576;
  constexpr uint64 GB = 1073741824;

  constexpr float to_kB(uint64 bytes) { return bytes / static_cast<float>(kB); }
  constexpr float to_MB(uint64 bytes) { return to_kB(bytes) / static_cast<float>(kB); }
  constexpr float to_GB(uint64 bytes) { return to_MB(bytes) / static_cast<float>(kB); }

  class ScopeGuard
  {
  public:
    template<class Callable>
    ScopeGuard(Callable &&func) : _func(std::forward<Callable>(func))
    {
    }
    ~ScopeGuard()
    {
      if (_func)
        _func();
    }
    ScopeGuard(ScopeGuard &&other) : _func(std::move(other._func)) { other._func == nullptr; }
    void reset() { _func = nullptr; }

    ScopeGuard(const ScopeGuard &sg) = delete;
    void operator=(const ScopeGuard &sg) = delete;

  private:
    std::function<void()> _func;
  };
}// namespace utility
}// namespace ningine
