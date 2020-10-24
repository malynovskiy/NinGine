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

    //explicit Vector2(const Point2<T> &point);
    //explicit Vector2(const Point3<T> &point);

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

    //explicit Vector3(const Point3<T> &point);

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

    Vector3<T>& normalize() { return *this = Normalize(*this); }

    T x, y, z;
  };

  template<typename T> inline Vector3<T> operator*(T t, const Vector3<T> &v) { return v * t; }

  template<typename T> inline Vector3<T> Abs(const Vector3<T> &v)
  {
    return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
  }

  template<typename T> inline T Dot(const Vector3<T> &left, const Vector3<T> &right)
  {
    return left.x * right.x + left.y * right.y + left.z * right.z;
  }

  template<typename T> inline T AbsDot(const Vector3<T> &left, const Vector3<T> &right)
  {
    return std::abs(Dot(left, right));
  }

  template<typename T> inline Vector3<T> Cross(const Vector3<T> &left, const Vector3<T> &right)
  {
    const double lx = left.x, ly = left.y, lz = left.z;
    const double rx = right.x, ry = right.y, rz = right.z;

    return Vector3<T>((ly * rz) - (lz * ry), (lz * rx) - (lx * rz), (lx * ry) - (ly * rx));
  }

  template<typename T> inline Vector3<T> Normalize(const Vector3<T> &v) { return v / v.Length(); }

  template<typename T> T MinComponent(const Vector3<T> &v) { return std::min(v.x, std::min(v.y, v.z)); }

  template<typename T> T MaxComponent(const Vector3<T> &v) { return std::max(v.x, std::max(v.y, v.z)); }

  template<typename T> int MaxDimension(const Vector3<T> &v)
  {
    return (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) : ((v.y > v.z) ? 1 : 2);
  }

  template<typename T> Vector3<T> Min(const Vector3<T> &p1, const Vector3<T> &p2)
  {
    return Vector3<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
  }
  template<typename T> Vector3<T> Max(const Vector3<T> &p1, const Vector3<T> &p2)
  {
    return Vector3<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
  }

  template<typename T> Vector3<T> Permute(const Vector3<T> &v, int x, int y, int z)
  {
    return Vector3<T>(v[x], v[y], v[z]);
  }

  // [different from original}
  template<typename T> 
  Vector3<T> CoordinateSystems(const Vector3<T> &v1, Vector3<T> &v2, Vector3<T> &v3)
  {
    if(std::abs(v1.x) > std::abs(v1.y))
      v2 = Vector3<T>(-v1.z, 0, v1.x).normalize();
    else
      v2 = Vector3<T>(0, v1.z, -v1.y).normalize();
    
  }

  typedef Vector2<float> Vector2f;
  typedef Vector2<int> Vector2i;
  typedef Vector3<float> Vector3f;
  typedef Vector3<float> Vector3i;

}// namespace math
}// namespace ningine
