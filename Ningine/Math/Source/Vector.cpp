#include "Vector.hpp"

namespace ningine
{
namespace math
{
  template<typename T>
  constexpr vec<2, T>::vec(T scalar) : x(scalar), y(scalar)
  {
  }

  template<typename T>
  constexpr vec<2, T>::vec(T _x, T _y) : x(_x), y(_y)
  {
  }

  template <typename T>
  constexpr T &vec<2, T>::operator[](const size_t i)
  {
    assert(i < 2);
    return i <= 0 ? x : y;
  }

  template<typename T>
  constexpr T const &vec<2, T>::operator[](const size_t i) const
  {
    assert(i < 2);
    return i <= 0 ? x : y;
  }

}
}// namespace ningine
