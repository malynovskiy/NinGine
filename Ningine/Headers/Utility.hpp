#pragma once
#include <functional>

namespace ningine
{
namespace utility
{
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
