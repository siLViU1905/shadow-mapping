#ifndef __MAT_TRANSFORM_H__
#define __MAT_TRANSFORM_H__
#include "Mat4.hpp"
#include "../vector/Vec3.hpp"
#include <cmath>

namespace sml
{
    template <typename ty>
    constexpr mat<ty, 4, 4> translate(const mat<ty, 4, 4> &m, const vec<ty, 3> &v)
    {
        mat<ty, 4, 4> result = m;

        result.mMat[3][0] = m.mMat[0][0] * v.x + m.mMat[1][0] * v.y + m.mMat[2][0] * v.z + m.mMat[3][0];
        result.mMat[3][1] = m.mMat[0][1] * v.x + m.mMat[1][1] * v.y + m.mMat[2][1] * v.z + m.mMat[3][1];
        result.mMat[3][2] = m.mMat[0][2] * v.x + m.mMat[1][2] * v.y + m.mMat[2][2] * v.z + m.mMat[3][2];

        return result;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> scale(const mat<ty, 4, 4> &m, const vec<ty, 3> &v)
    {
        mat<ty, 4, 4> result = m;

        for (int i = 0; i < 3; ++i)
        {
            result.mMat[i][0] *= v.x;
            result.mMat[i][1] *= v.y;
            result.mMat[i][2] *= v.z;
        }

        return result;
    }

    template <typename ty>
    constexpr ty as_radians(ty degrees)
    {
        return degrees * (ty(3.1415) / ty(180));
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> rotate(const mat<ty, 4, 4> &m, ty angle, const vec<ty, 3> &v)
    {
        vec<ty, 3> normalized = normalize(v);

        ty x = normalized.x;
        ty y = normalized.y;
        ty z = normalized.z;

        ty s = std::sin(angle);
        ty c = std::cos(angle);
        ty t = ty(1) - c;

        ty xx = x * x;
        ty xy = x * y;
        ty xz = x * z;
        ty yy = y * y;
        ty yz = y * z;
        ty zz = z * z;

        mat<ty, 4, 4> rotMat;

        // First row
        rotMat.mMat[0][0] = c + xx * t;
        rotMat.mMat[0][1] = xy * t - z * s;
        rotMat.mMat[0][2] = xz * t + y * s; // THIS WAS MISSING IN YOUR CODE
        rotMat.mMat[0][3] = static_cast<ty>(0);

        // Second row
        rotMat.mMat[1][0] = xy * t + z * s;
        rotMat.mMat[1][1] = c + yy * t;
        rotMat.mMat[1][2] = yz * t - x * s;
        rotMat.mMat[1][3] = static_cast<ty>(0);

        // Third row
        rotMat.mMat[2][0] = xz * t - y * s;
        rotMat.mMat[2][1] = yz * t + x * s;
        rotMat.mMat[2][2] = c + zz * t;
        rotMat.mMat[2][3] = static_cast<ty>(0);

        // Fourth row
        rotMat.mMat[3][0] = static_cast<ty>(0);
        rotMat.mMat[3][1] = static_cast<ty>(0);
        rotMat.mMat[3][2] = static_cast<ty>(0);
        rotMat.mMat[3][3] = static_cast<ty>(1);

        return rotMat * m;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> perspective(ty fov, ty aspect, ty znear, ty zfar)
    {
        mat<ty, 4, 4> result(0);

        ty f = ty(1) / std::tan(fov / ty(2));

        result.mMat[0][0] = f / aspect;

        result.mMat[1][1] = f;

        result.mMat[2][2] = -(zfar + znear) / (zfar - znear);
        result.mMat[3][2] = -(ty(2) * zfar * znear) / (zfar - znear);

        result.mMat[2][3] = -ty(1);
        result.mMat[3][3] = ty(0);

        return result;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> look_at(const vec<ty, 3> &eye, const vec<ty, 3> &center, const vec<ty, 3> &up)
    {
        vec<ty, 3> f = normalize(center - eye);
        vec<ty, 3> s = normalize(cross(f, normalize(up)));
        vec<ty, 3> u = cross(s, f);

        mat<ty, 4, 4> result(1);

        result.mMat[0][0] = s.x;
        result.mMat[0][1] = u.x;
        result.mMat[0][2] = -f.x;
        result.mMat[0][3] = ty(0);

        result.mMat[1][0] = s.y;
        result.mMat[1][1] = u.y;
        result.mMat[1][2] = -f.y;
        result.mMat[1][3] = ty(0);

        result.mMat[2][0] = s.z;
        result.mMat[2][1] = u.z;
        result.mMat[2][2] = -f.z;
        result.mMat[2][3] = ty(0);

        result.mMat[3][0] = -dot(s, eye);
        result.mMat[3][1] = -dot(u, eye);
        result.mMat[3][2] = dot(f, eye);
        result.mMat[3][3] = ty(1);

        return result;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> ortho(ty left, ty right, ty bottom, ty top, ty zNear, ty zFar)
    {
        mat<ty, 4, 4> result(ty(1));

        result.mMat[0][0] = static_cast<ty>(2) / (right - left);
        result.mMat[1][1] = static_cast<ty>(2) / (top - bottom);
        result.mMat[2][2] = -static_cast<ty>(2) / (zFar - zNear);
        result.mMat[3][0] = -(right + left) / (right - left);
        result.mMat[3][1] = -(top + bottom) / (top - bottom);
        result.mMat[3][2] = -(zFar + zNear) / (zFar - zNear);

        return result;
    }

}

#endif // __MAT_TRANSFORM_H__