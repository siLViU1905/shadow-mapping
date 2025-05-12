#ifndef __VEC3_H__
#define __VEC3_H__
#include "Vec.hpp"
#include <stdexcept>
#include <cmath>

namespace sml
{
  template <typename ty>
  class vec<ty, 3>
  {
  public:
    ty x, y, z;

    constexpr vec() : x(0), y(0), z(0) {}

    constexpr vec(ty scalar) : x(scalar), y(scalar), z(scalar) {}

    constexpr vec(ty x, ty y, ty z) : x(x), y(y), z(z) {}

    constexpr vec(const vec &other) : x(other.x), y(other.y), z(other.z) {}

    constexpr vec(vec &&other)
    {
      x = std::move(other.x);
      y = std::move(other.y);
      z = std::move(other.z);
    }

    constexpr vec &operator=(const vec &other)
    {
      x = other.x;
      y = other.y;
      z = other.z;
      return *this;
    }

    constexpr vec &operator=(vec &&other)
    {
      x = std::move(other.x);
      y = std::move(other.y);
      z = std::move(other.z);
      return *this;
    }

    constexpr ty &operator[](unsigned int index)
    {
      if (index > 2)
        throw std::runtime_error("Out of range");
      return *(&x + index);
    }

    constexpr const ty &operator[](unsigned int index) const
    {
      if (index > 2)
        throw std::runtime_error("Out of range");
      return *(&x + index);
    }
  };

  typedef vec<float, 3> vec3;
  typedef vec<int, 3> ivec3;
  typedef vec<double, 3> dvec3;

  template <typename ty>
  constexpr vec<ty, 3> operator+(const vec<ty, 3> &lhs, const vec<ty, 3> &rhs)
  {
    return vec<ty, 3>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
  }

  template <typename ty>
  constexpr vec<ty, 3> operator+(const vec<ty, 3> &lhs, vec<ty, 3> &&rhs)
  {
    return vec<ty, 3>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
  }

  template <typename ty>
  constexpr vec<ty, 3> operator+(const vec<ty, 3> &lhs, ty scalar)
  {
    return vec<ty, 3>(lhs.x + scalar, lhs.y + scalar, lhs.z + scalar);
  }

  template <typename ty>
  constexpr vec<ty, 3> operator-(const vec<ty, 3> &lhs, const vec<ty, 3> &rhs)
  {
    return vec<ty, 3>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
  }

  template <typename ty>
  constexpr vec<ty, 3> operator-(const vec<ty, 3> &lhs, vec<ty, 3> &&rhs)
  {
    return vec<ty, 3>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
  }

  template <typename ty>
  constexpr vec<ty, 3> operator-(const vec<ty, 3> &lhs, ty scalar)
  {
    return vec<ty, 3>(lhs.x - scalar, lhs.y - scalar, lhs.z - scalar);
  }

  template <typename ty>
  constexpr vec<ty, 3> &operator+=(vec<ty, 3> &lhs, const vec<ty, 3> &rhs)
  {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
  }

  template <typename ty>
  constexpr vec<ty, 3> &operator+=(vec<ty, 3> &lhs, vec<ty, 3> &&rhs)
  {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
  }

  template <typename ty>
  constexpr vec<ty, 3> &operator+=(vec<ty, 3> &lhs, float scalar)
  {
    lhs.x += scalar;
    lhs.y += scalar;
    lhs.z += scalar;
    return lhs;
  }

  template <typename ty>
  constexpr vec<ty, 3> &operator-=(vec<ty, 3> &lhs, const vec<ty, 3> &rhs)
  {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
  }

  template <typename ty>
  constexpr vec<ty, 3> &operator-=(vec<ty, 3> &lhs, vec<ty, 3> &&rhs)
  {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
  }

  template <typename ty>
  constexpr vec<ty, 3> &operator-=(vec<ty, 3> &lhs, float scalar)
  {
    lhs.x -= scalar;
    lhs.y -= scalar;
    lhs.z -= scalar;
    return lhs;
  }

  template <typename ty>
  constexpr vec<ty, 3> operator*(const vec<ty, 3> &lhs, float sclalar)
  {
    return vec<ty, 3>(lhs.x * sclalar, lhs.y * sclalar, lhs.z * sclalar);
  }

  template <typename ty>
  constexpr vec<ty, 3> &operator*=(vec<ty, 3> &lhs, float sclalar)
  {
    lhs.x *= sclalar;
    lhs.y *= sclalar;
    lhs.z *= sclalar;
    return lhs;
  }

  template <typename ty>
  constexpr vec<ty, 3> operator/(const vec<ty, 3> &lhs, float sclalar)
  {
    return vec<ty, 3>(lhs.x / sclalar, lhs.y / sclalar, lhs.z / sclalar);
  }

  template <typename ty>
  constexpr vec<ty, 3> &operator/=(vec<ty, 3> &lhs, float sclalar)
  {
    lhs.x /= sclalar;
    lhs.y /= sclalar;
    lhs.z /= sclalar;
    return lhs;
  }

  template <typename ty>
  constexpr vec<ty, 3> perpendicular(const vec<ty, 3> &v)
  {

    if (std::abs(v.y) > ty(0) || std::abs(v.z) > ty(0))
      return vec<ty, 3>(v.y + v.z, -v.x, -v.x);
    else
      return vec<ty, 3>(ty(0), ty(1), ty(0));
  }

  template <typename ty>
  constexpr vec<ty, 3> normalize(const vec<ty, 3> &v)
  {
    ty length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == ty(0))
      return vec<ty, 3>();
    return vec<ty, 3>(v.x / length, v.y / length, v.z / length);
  }

  template <typename ty>
  constexpr ty dot(const vec<ty, 3> &v1, const vec<ty, 3> &v2)
  {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
  }

  template <typename ty>
  constexpr vec<ty, 3> cross(const vec<ty, 3> &v1, const vec<ty, 3> &v2)
  {
    return vec<ty, 3>(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x);
  }
}

#endif // __VEC3_H__