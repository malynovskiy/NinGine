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
    Vector2(T X, T Y) : x(X), y(Y) { assert(!hasNaNs()); }

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

    Vector2<T> operator+(const Vector2<T> &v) const
    {
      assert(!v.hasNaNs());
      return Vector2<T>(x + v.x, y + v.y);
    }

    Vector2<T> operator-(const Vector2<T> &v) const
    {
      assert(!v.hasNaNs());
      return Vector2<T>(x - v.x, y - v.y);
    }

    Vector2<T> &operator+=(const Vector2<T> &v)
    {
      assert(!v.hasNaNs());
      x += v.x;
      y += v.y;
      return *this;
    }

    Vector2<T> &operator-=(const Vector2<T> &v)
    {
      assert(!v.hasNaNs());
      x -= v.x;
      y -= v.y;
      return *this;
    }

    bool operator==(const Vector2<T> &v) const { return x == v.x && y == v.y; }
    bool operator!=(const Vector2<T> &v) const { return x != v.x || y != v.y; }

    Vector2<T> operator*(T t) const { return Vector2(x * t, y * t); }
    Vector2<T> &operator*=(T t)
    {
      assert(!std::isnan(t));
      x *= t;
      y *= t;
      return *this;
    }

    Vector2<T> operator/(T t) const
    {
      assert(!std::isnan(t));
      float inv = 1.0f / t;
      return Vector2(x * inv, y * inv);
    }

    Vector2<T> &operator/=(T t)
    {
      assert(!std::isnan(t));
      float inv = 1.0f / t;
      x *= inv;
      y *= inv;
      return *this;
    }

    Vector2<T> operator-() const { return Vector2<T>(-x, -y); }

    Vector2<T> operator[](size_t i) const
    {
      assert(i <= 1);
      if (i == 0)
        return x;
      return y;
    }

    Vector2<T> &operator[](size_t i)
    {
      assert(i <= 1);
      if (i == 0)
        return x;
      return y;
    }

    float LengthSquared() const { return x * x + y * y; }
    float Length() const { return std::sqrt(LengthSquared()); }

    T x, y;
  };

  template<typename T> struct Vector3
  {
    Vector3() { x = y = z = 0; }
    Vector3(T X, T Y, T Z) : x(X), y(Y), z(Z) { assert(!hasNaNs()); }

    bool hasNaNs() const { return std::isnan(x) || std::isnan(y) || std::isnan(z); }

    explicit Vector3(const Point3<T> &point);

#ifndef NDEBUG
    Vector3(const Vector3<T> &v)
    {
      assert(!v.hasNaNs());
      x = v.x;
      y = v.y;
      z = v.z;
    }
    Vector3<T> &operator=(const Vector3<T> &v)
    {
      assert(!v.hasNaNs());
      x = v.x;
      y = v.y;
      z = v.z;
      return *this;
    }
#endif

    friend std::ostream &operator<<(std::ostream &os, const Vector3<T> &v)
    {
      os << '[' << v.x << ", " << v.y << ", " << v.z << ']';
      return os;
    }

    Vector3<T> operator+(const Vector3<T> &v) const
    {
      assert(!v.hasNaNs());
      return Vector3<T>(x + v.x, y + v.y, z + v.z);
    }

    Vector3<T> operator-(const Vector3<T> &v) const
    {
      assert(!v.hasNaNs());
      return Vector3<T>(x - v.x, y - v.y, z - v.z);
    }

    Vector3<T> &operator+=(const Vector3<T> &v)
    {
      assert(!v.hasNaNs());
      x += v.x;
      y += v.y;
      z += v.z;
      return *this;
    }

    Vector3<T> &operator-=(const Vector3<T> &v)
    {
      assert(!v.hasNaNs());
      x -= v.x;
      y -= v.y;
      z -= v.z;
      return *this;
    }

    bool operator==(const Vector3<T> &v) const { return x == v.x && y == v.y && z == v.z; }
    bool operator!=(const Vector3<T> &v) const { return x != v.x || y != v.y || z != v.z; }

    Vector3<T> operator*(T t) const { return Vector3(x * t, y * t, z * t); }
    Vector3<T> &operator*=(T t)
    {
      assert(!std::isnan(t));
      x *= t;
      y *= t;
      z *= t;
      return *this;
    }

    Vector3<T> operator/(T t) const
    {
      assert(!std::isnan(t));
      float inv = 1.0f / t;
      return Vector3(x * inv, y * inv, z * inv);
    }

    Vector3<T> &operator/=(T t)
    {
      assert(!std::isnan(t));
      float inv = 1.0f / t;
      x *= inv;
      y *= inv;
      z *= inv;
      return *this;
    }

    Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }

    Vector3<T> operator[](size_t i) const
    {
      assert(i <= 2);
      if (i == 0)
        return x;
      if (i == 1)
        return y;
      return z;
    }

    Vector3<T> &operator[](size_t i)
    {
      assert(i <= 2);
      if (i == 0)
        return x;
      if (i == 1)
        return y;
      return z;
    }

    float LengthSquared() const { return x * x + y * y + z * z; }
    float Length() const { return std::sqrt(LengthSquared()); }

    T x, y, z;
  };
}// namespace math
}// namespace ningine
