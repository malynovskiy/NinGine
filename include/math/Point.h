#include "Vector.h"

namespace ningine
{
namespace math
{
  template<typename T> struct Point2
  {
    Point2() : x(0), y(0) {}
    Point2(T X, T Y) : x(X), y(Y) { assert(!hasNaNs()); }
    explicit Point2(const Point3<T> &p) : x(p.x), y(p.y) { assert(!hasNaNs()); }

    template<typename U> explicit Point2(const Point2<U> &p)
    {
      x = (T)p.x;
      y = (T)p.y;
      Assert(!hasNaNs());
    }

    template<typename U> explicit Point2(const Vector2<U> &p)
    {
      x = (T)p.x;
      y = (T)p.y;
      Assert(!hasNaNs());
    }

    template<typename U> explicit operator Vector2<U>() const { return Vector2<U>(x, y); }

#ifndef NDEBUG
    Point2(const Point2<T> &p)
    {
      Assert(!p.hasNaNs());
      x = p.x;
      y = p.y;
    }

    Point2<T> &operator=(const Point2<T> &p)
    {
      Assert(!p.hasNaNs());
      x = p.x;
      y = p.y;
      return *this;
    }
#endif// !NDEBUG
    friend std::ostream &operator<<(std::ostream &os, const Point2<T> &p)
    {
      os << "[" << p.x << ", " << p.y << "]";
      return os;
    }

    Point2<T> operator+(const Vector2<T> &v) const
    {
      Assert(!v.hasNaNs());
      return Point2<T>(x + v.x, y + v.y);
    }

    Point2<T> &operator+=(const Vector2<T> &v)
    {
      Assert(!v.hasNaNs());
      x += v.x;
      y += v.y;
      return *this;
    }
    Vector2<T> operator-(const Point2<T> &p) const
    {
      Assert(!p.hasNaNs());
      return Vector2<T>(x - p.x, y - p.y);
    }

    Point2<T> operator-(const Vector2<T> &v) const
    {
      Assert(!v.hasNaNs());
      return Point2<T>(x - v.x, y - v.y);
    }

    Point2<T> operator-() const { return Point2<T>(-x, -y); }

    Point2<T> &operator-=(const Vector2<T> &v)
    {
      Assert(!v.hasNaNs());
      x -= v.x;
      y -= v.y;
      return *this;
    }

    Point2<T> &operator+=(const Point2<T> &p)
    {
      Assert(!p.hasNaNs());
      x += p.x;
      y += p.y;
      return *this;
    }

    Point2<T> operator+(const Point2<T> &p) const
    {
      Assert(!p.hasNaNs());
      return Point2<T>(x + p.x, y + p.y);
    }

    Point2<T> operator*(T f) const { return Point2<T>(f * x, f * y); }

    Point2<T> &operator*=(T f)
    {
      x *= f;
      y *= f;
      return *this;
    }

    Point2<T> operator/(T f) const
    {
      float inv = 1.0f / f;
      return Point2<T>(inv * x, inv * y);
    }

    Point2<T> &operator/=(T f)
    {
      float inv = 1.0f / f;
      x *= inv;
      y *= inv;
      return *this;
    }

    T operator[](size_t i) const
    {
      assert(i <= 1);
      if (i == 0)
        return x;
      return y;
    }

    T &operator[](size_t i)
    {
      Assert(i <= 1);
      if (i == 0)
        return x;
      return y;
    }

    bool operator==(const Point2<T> &p) const { return x == p.x && y == p.y; }
    bool operator!=(const Point2<T> &p) const { return x != p.x || y != p.y; }
    bool hasNaNs() const { return std::isnan(x) || std::isnan(y); }

    T x, y;
  };

  template<typename T> struct Point3
  {
    Point3() : x(0), y(0), z(0) {}
    Point3(T X, T Y, T Z) : x(X), y(Y), z(Z) { assert(!hasNaNs()); }
    template<typename U> explicit Point3(const Point3<U> &p) : x((T)p.x), y((T)p.y), z((T)p.z) { assert(!hasNaNs()); }
    template<typename U> explicit operator Vector3<U>() const { return Vector3<U>(x, y, z); }

#ifndef NDEBUG
    Point3(const Point3<T> &p)
    {
      Assert(!p.hasNaNs());
      x = p.x;
      y = p.y;
      z = p.z;
    }

    Point3<T> &operator=(const Point3<T> &p)
    {
      Assert(!p.hasNaNs());
      x = p.x;
      y = p.y;
      z = p.z;
      return *this;
    }
#endif// !NDEBUG

    friend std::ostream &operator<<(std::ostream &os, const Point3<T> &p)
    {
      os << "[" << p.x << ", " << p.y << ", " << p.z << "]";
      return os;
    }

    Point3<T> operator+(const Vector3<T> &v) const { return Point3<T>(x + v.x, y + v.y, z + v.z); }
    Point3<T> &operator+=(const Vector3<T> &v)
    {
      x += v.x;
      y += v.y;
      z += v.z;
      return *this;
    }

    Vector3<T> operator-(const Point3<T> &p) const { return Vector3<T>(x - p.x, y - p.y, z - p.z); }
    Point3<T> operator-(const Vector3<T> &v) const { return Point3<T>(x - v.x, y - v.y, z - v.z); }

    Point3<T> &operator-=(const Vector3<T> &v)
    {
      x -= v.x;
      y -= v.y;
      z -= v.z;
      return *this;
    }

    Point3<T> &operator+=(const Point3<T> &p)
    {
      x += p.x;
      y += p.y;
      z += p.z;
      return *this;
    }

    Point3<T> operator+(const Point3<T> &p) const { return Point3<T>(x + p.x, y + p.y, z + p.z); }
    Point3<T> operator*(T f) const { return Point3<T>(f * x, f * y, f * z); }
    Point3<T> &operator*=(T f)
    {
      x *= f;
      y *= f;
      z *= f;
      return *this;
    }

    Point3<T> operator/(T f) const
    {
      float inv = 1.0f / f;
      return Point3<T>(inv * x, inv * y, inv * z);
    }

    Point3<T> &operator/=(T f)
    {
      float inv = 1.0f / f;
      x *= inv;
      y *= inv;
      z *= inv;
      return *this;
    }

    T operator[](size_t i) const
    {
      assert(i <= 2);
      if (i == 0)
        return x;
      if (i == 1)
        return y;
      return z;
    }

    T &operator[](size_t i)
    {
      assert(i <= 2);
      if (i == 0)
        return x;
      if (i == 1)
        return y;
      return z;
    }

    bool operator==(const Point3<T> &p) const { return x == p.x && y == p.y && z == p.z; }
    bool operator!=(const Point3<T> &p) const { return x != p.x || y != p.y || z != p.z; }
    bool hasNaNs() const { return std::isnan(x) || std::isnan(y) || std::isnan(z); }
    Point3<T> operator-() const { return Point3<T>(-x, -y, -z); }

    T x, y, z;
  };

  typedef Point2<float> Point2f;
  typedef Point2<int> Point2i;
  typedef Point3<float> Point3f;
  typedef Point3<int> Point3i;

  template<typename T> Point3<T> Lerp(float t, const Point3<T> &p1, const Point3<T> &p2)
  {
    return (1.0f - t) * p1 + t * p2;
  }

  template<typename T> Point3<T> Min(const Point3<T> &p1, const Point3<T> &p2)
  {
    return Point3<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
  }

  template<typename T> Point3<T> Max(const Point3<T> &p1, const Point3<T> &p2)
  {
    return Point3<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
  }

  template<typename T> Point3<T> Floor(const Point3<T> &p)
  {
    return Point3<T>(std::floor(p.x), std::floor(p.y), std::floor(p.z));
  }

  template<typename T> Point3<T> Ceil(const Point3<T> &p)
  {
    return Point3<T>(std::ceil(p.x), std::ceil(p.y), std::ceil(p.z));
  }

  template<typename T> Point3<T> Abs(const Point3<T> &p)
  {
    return Point3<T>(std::abs(p.x), std::abs(p.y), std::abs(p.z));
  }

  template<typename T> Point3<T> Permute(const Point3<T> &p, int x, int y, int z)
  {
    return Point3<T>(p[x], p[y], p[z]);
  }
}// namespace math
}// namespace ningine
