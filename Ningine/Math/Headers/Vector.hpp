#pragma once
#include <vector>
#include <cassert>
#include <ostream>
#include <cmath>

namespace ningine
{
namespace math
{
  template<size_t DIM, typename T>
  struct vec
  {
    vec()
    {
      for (size_t i = DIM; i--; _data[i] = T())
        ;
    }
    T &operator[](const size_t i)
    {
      assert(i < DIM);
      return _data[i];
    }
    const T &operator[](const size_t i) const
    {
      assert(i < DIM);
      return _data[i];
    }

    static constexpr int length() { return DIM; }
  private:
    T _data[DIM];
  };

  typedef vec<2, int> Vec2i;
  typedef vec<2, unsigned int> Vec2ui;
  typedef vec<2, float> Vec2f;
  typedef vec<3, float> Vec3f;
  typedef vec<4, float> Vec4f;

  template<typename T>
  struct vec<2, T>
  {
    typedef vec<2, T> type;

    constexpr vec() = default;
    constexpr vec(const type &other) = default;

    constexpr explicit vec(T scalar);
    constexpr vec(T x, T y);

    template<class U>
    vec<2, T>(const vec<2, U> &v);

    constexpr T &operator[](const size_t i);
    constexpr T const &operator[](const size_t i) const;
    
    static constexpr size_t length() { return 2; }

    T x, y;
  };

  template<typename T>
  struct vec<3, T>
  {
    typedef vec<3, T> type;

    constexpr vec() = default;
    constexpr vec(const type &other) = default;

    vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

    T &operator[](const size_t i)
    {
      assert(i < 3);
      return i <= 0 ? x : (1 == i ? y : z);
    }
    const T &operator[](const size_t i) const
    {
      assert(i < 3);
      return i <= 0 ? x : (1 == i ? y : z);
    }
    static constexpr int length() { return 3; }

    float norm() { return std::sqrt(x * x + y * y + z * z); }
    vec<3, T> &normalize(T l = 1)
    {
      *this = (*this) * (l / norm());
      return *this;
    }

    T x, y, z;
  };

  template<typename T> struct vec<4, T>
  {
    vec() : x(T()), y(T()), z(T()), w(T()) {}
    vec(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}

    T &operator[](const size_t i)
    {
      assert(i < 4);
      return i <= 0 ? x : (1 == i ? y : (2 == i ? z : w));
    }
    const T &operator[](const size_t i) const
    {
      assert(i < 4);
      return i <= 0 ? x : (1 == i ? y : (2 == i ? z : w));
    }

    T x, y, z, w;
  };

  // Addition
  template<size_t DIM, typename T> vec<DIM, T> operator+(vec<DIM, T> lhs, const vec<DIM, T> &rhs)
  {
    for (size_t i = DIM; i--; lhs[i] += rhs[i])
      ;
    return lhs;
  }

  // Substraction
  template<size_t DIM, typename T> vec<DIM, T> operator-(vec<DIM, T> lhs, const vec<DIM, T> &rhs)
  {
    for (size_t i = DIM; i--; lhs[i] -= rhs[i])
      ;
    return lhs;
  }

  // Dot product
  template<size_t DIM, typename T> T operator*(const vec<DIM, T> &lhs, const vec<DIM, T> &rhs)
  {
    T ret = T();
    for (size_t i = DIM; i--; ret += lhs[i] * rhs[i])
      ;
    return ret;
  }

  // Product of the vector with scalar
  template<size_t DIM, typename T, typename U>
  vec<DIM, T> operator*(const vec<DIM, T> lhs, const U &scalar)
  {
    vec<DIM, T> ret;
    for (size_t i = DIM; i--; ret[i] = lhs[i] * scalar)
      ;
    return ret;
  }

  // Oposite
  template<size_t DIM, typename T> vec<DIM, T> operator-(const vec<DIM, T> &lhs)
  {
    return lhs * T(-1);
  }

  // Cross product
  template<typename T> vec<3, T> cross(vec<3, T> v1, vec<3, T> v2)
  {
    return vec<3, T>(
      v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
  }

  template<size_t DIM, typename T> std::ostream &operator<<(std::ostream &out, const vec<DIM, T> &v)
  {
    for (size_t i = 0; i < DIM; ++i) out << v[i] << ' ';

    return out;
  }
}// namespace math
}// namespace ningine
