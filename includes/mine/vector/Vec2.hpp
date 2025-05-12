#ifndef __VEC2_H__
#define __VEC2_H__
#include "Vec.hpp"
#include <cmath>
#include <stdexcept>  

namespace sml
{
    template <typename ty>
    class vec<ty, 2>
    {
    public:
        ty x, y;

        constexpr vec() : x(0), y(0) {}

        constexpr vec(ty scalar) : x(scalar), y(scalar) {}

        constexpr vec(ty x, ty y) : x(x), y(y) {}

        constexpr vec(const vec &other) : x(other.x), y(other.y) {}

        constexpr vec(vec &&other)
        {
            x = std::move(other.x);
            y = std::move(other.y);
        }

        constexpr vec &operator=(const vec &other)
        {
            x = other.x;
            y = other.y;
            return *this;
        }

        constexpr vec &operator=(vec &&other)
        {
            x = std::move(other.x);
            y = std::move(other.y);
            return *this;
        }

        constexpr ty& operator[](unsigned int index)
        {
            if(index>1)
             throw std::runtime_error("Out of range");
             return *(&x + index);
        }

        constexpr const ty& operator[](unsigned int index) const
        {
            if(index>1)
             throw std::runtime_error("Out of range");
             return *(&x + index);
        }
    };

    typedef vec<float, 2> vec2;
    typedef vec<int, 2> ivec2;
    typedef vec<double, 2> dvec2;

    template <typename ty>
    constexpr vec<ty, 2> operator+(const vec<ty, 2> &lhs, const vec<ty, 2> &rhs)
    {
        return vec<ty, 2>(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    template <typename ty>
    constexpr vec<ty, 2> operator+(const vec<ty, 2> &lhs, vec<ty, 2> &&rhs)
    {
        return vec<ty, 2>(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    template <typename ty>
    constexpr vec<ty, 2> operator-(const vec<ty, 2> &lhs, const vec<ty, 2> &rhs)
    {
        return vec<ty, 2>(lhs.x - rhs.x, lhs.y - rhs.y);
    }

    template <typename ty>
    constexpr vec<ty, 2> operator-(const vec<ty, 2> &lhs, vec<ty, 2> &&rhs)
    {
        return vec<ty, 2>(lhs.x - rhs.x, lhs.y - rhs.y);
    }

    template <typename ty>
    constexpr vec<ty, 2> &operator+=(vec<ty, 2> &lhs, const vec<ty, 2> &rhs)
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }

    template <typename ty>
    constexpr vec<ty, 2> &operator+=(vec<ty, 2> &lhs, vec<ty, 2> &&rhs)
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }

    template <typename ty>
    constexpr vec<ty, 2> &operator-=(vec<ty, 2> &lhs, const vec<ty, 2> &rhs)
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        return lhs;
    }

    template <typename ty>
    constexpr vec<ty, 2> &operator-=(vec<ty, 2> &lhs, vec<ty, 2> &&rhs)
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        return lhs;
    }

    template <typename ty>
    constexpr vec<ty, 2> operator*(const vec<ty, 2> &lhs, float sclalar)
    {
        return vec<ty, 2>(lhs.x * sclalar, lhs.y * sclalar);
    }

    template <typename ty>
    constexpr vec<ty, 2> &operator*=(vec<ty, 2> &lhs, float sclalar)
    {
        lhs.x *= sclalar;
        lhs.y *= sclalar;
        return lhs;
    }

    template <typename ty>
    constexpr vec<ty, 2> operator/(const vec<ty, 2> &lhs, float sclalar)
    {
        return vec<ty, 2>(lhs.x / sclalar, lhs.y / sclalar);
    }

    template <typename ty>
    constexpr vec<ty, 2> &operator/=(vec<ty, 2> &lhs, float sclalar)
    {
        lhs.x /= sclalar;
        lhs.y /= sclalar;
        return lhs;
    }

    template <typename ty>
    constexpr vec<ty, 2> perpendicular(const vec<ty, 2> &v)
    {
        return vec<ty, 2>(-v.y, v.x);
    }

    template <typename ty>
    constexpr vec<ty, 2> normalize(const vec<ty, 2> &v)
    {
        ty length = std::sqrt(v.x * v.x + v.y * v.y);
        if (length == ty(0))
            return vec<ty, 2>();
        return vec<ty, 2>(v.x / length, v.y / length);
    }

    template <typename ty>
    constexpr ty dot(const vec<ty, 2> &v1, const vec<ty, 2> &v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    template <typename ty>
    constexpr ty cross(const vec<ty, 2> &v1, const vec<ty, 2> &v2)
    {
        return v1.x * v2.y - v1.y * v2.x;
    }
}

#endif // __VEC2_H__