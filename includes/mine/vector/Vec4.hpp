#ifndef __VEC4_H__
#define __VEC4_H__
#include "Vec.hpp"

namespace sml
{
    template <typename ty>
    class vec<ty, 4>
    {
    public:
        ty x, y, z, w;

        constexpr vec() : x(0), y(0), z(0), w(0) {}

        constexpr vec(ty scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}

        constexpr vec(ty x, ty y, ty z, ty w) : x(x), y(y), z(z), w(w) {}

        constexpr vec(const vec &other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

        constexpr vec(vec &&other)
        {
            x = std::move(other.x);
            y = std::move(other.y);
            z = std::move(other.z);
            w = std::move(other.w);
        }

        constexpr vec &operator=(const vec &other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            w = other.w;
            return *this;
        }

        constexpr vec &operator=(vec &&other)
        {
            x = std::move(other.x);
            y = std::move(other.y);
            z = std::move(other.z);
            w = std::move(other.w);
            return *this;
        }

        constexpr ty &operator[](unsigned int index)
        {
            if (index > 3)
                throw std::runtime_error("Out of range");
            return *(&x + index);
        }

        constexpr const ty &operator[](unsigned int index) const
        {
            if (index > 3)
                throw std::runtime_error("Out of range");
            return *(&x + index);
        }
    };

    typedef vec<float, 4> vec4;
    typedef vec<int, 4> ivec4;
    typedef vec<double, 4> dvec4;

    template <typename ty>
    constexpr vec<ty, 4> operator+(const vec<ty, 4> &lhs, const vec<ty, 4> &rhs)
    {
        return vec<ty, 4>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
    }

    template <typename ty>
    constexpr vec<ty, 4> operator+(const vec<ty, 4> &lhs, vec<ty, 4> &&rhs)
    {
        return vec<ty, 4>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
    }

    template <typename ty>
    constexpr vec<ty, 4> operator-(const vec<ty, 4> &lhs, const vec<ty, 4> &rhs)
    {
        return vec<ty, 4>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
    }

    template <typename ty>
    constexpr vec<ty, 4> operator-(const vec<ty, 4> &lhs, vec<ty, 4> &&rhs)
    {
        return vec<ty, 4>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
    }

    template <typename ty>
    constexpr vec<ty, 4> &operator+=(vec<ty, 4> &lhs, const vec<ty, 4> &rhs)
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        lhs.z += rhs.z;
        lhs.w += rhs.w;
        return lhs;
    }

    template <typename ty>
    constexpr vec<ty, 4> &operator+=(vec<ty, 4> &lhs, vec<ty, 4> &&rhs)
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        lhs.z += rhs.z;
        lhs.w += rhs.w;
        return lhs;
    }

    template <typename ty>
    constexpr vec<ty, 4> &operator-=(vec<ty, 4> &lhs, const vec<ty, 4> &rhs)
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        lhs.z -= rhs.z;
        lhs.w -= rhs.w;
        return lhs;
    }

    template <typename ty>
    constexpr vec<ty, 4> &operator-=(vec<ty, 4> &lhs, vec<ty, 4> &&rhs)
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        lhs.z -= rhs.z;
        lhs.w -= rhs.w;
        return lhs;
    }

    template <typename ty>
    constexpr vec<ty, 4> operator*(const vec<ty, 4> &lhs, float sclalar)
    {
        return vec<ty, 4>(lhs.x * sclalar, lhs.y * sclalar, lhs.z * sclalar, lhs.w * sclalar);
    }

    template <typename ty>
    constexpr vec<ty, 4> &operator*=(vec<ty, 4> &lhs, float sclalar)
    {
        lhs.x *= sclalar;
        lhs.y *= sclalar;
        lhs.z *= sclalar;
        lhs.w *= sclalar;
        return lhs;
    }

    template <typename ty>
    constexpr vec<ty, 4> operator/(const vec<ty, 4> &lhs, float sclalar)
    {
        return vec<ty, 4>(lhs.x / sclalar, lhs.y / sclalar, lhs.z / sclalar, lhs.w / sclalar);
    }

    template <typename ty>
    constexpr vec<ty, 4> &operator/=(vec<ty, 4> &lhs, float sclalar)
    {
        lhs.x /= sclalar;
        lhs.y /= sclalar;
        lhs.z /= sclalar;
        lhs.w /= sclalar;
        return lhs;
    }

    template <typename ty>
    constexpr vec<ty, 4> normalize(const vec<ty, 4> &v)
    {
        ty length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
        if (length == ty(0))
            return vec<ty, 4>();
        return vec<ty, 4>(v.x / length, v.y / length, v.z / length, v.w / length);
    }

    template <typename ty>
    constexpr ty dot(const vec<ty, 4> &v1, const vec<ty, 4> &v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }
}

#endif // __VEC4_H__