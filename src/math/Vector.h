#include <cmath>
#include <cassert>
#include <ostream>

namespace ningine
{
namespace math
{
  template<typename T> struct Vector2
  {
    Vector2() { x = y = 0; }
    Vector2(T X, T Y) x(X), y(Y) { assert(!hasNaNs()); }

    bool hasNaNs() const { return std::isnan(x) || std::isnan(y); }

    explicit Vector2(const Point2<T> &point);
    explicit Vector2(const Point3<T> &point);

#ifndef NDEBUG
    Vector2(const Vector2<T> &v)
    {
      assert(!v.hasNaNs());
      x = v.x;
      y = v.y;
    }
    Vector2<T> &operator=(const Vector2<T> &v)
    {
      assert(!v.hasNaNs());
      x = v.x;
      y = v.y;
      return *this;
    }
#endif

    friend std::ostream &operator<<(std::ostream &os, const Vector2<T> &v)
    {
      os << '[' << v.x << ", " << v.y << ']';
      return os;
    }

    Vector2<T> operator+(const Vector2<T> &v) {}

    T x, y;
  };
}// namespace math
}// namespace ningine
