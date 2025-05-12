#ifndef __MAT4_H__
#define __MAT4_H__
#include "Mat.hpp"
#include "../vector/Vec4.hpp"

namespace sml
{
    template <typename ty>
    class mat<ty, 4, 4>
    {
    public:
        ty mMat[4][4];

        constexpr mat()
        {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    mMat[i][j] = ty(0);
        }

        constexpr mat(ty scalar)
        {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    if (i == j)
                        mMat[i][j] = scalar;
                    else
                        mMat[i][j] = ty(0);
        }

        constexpr mat(const vec<ty, 4> &v1, const vec<ty, 4> &v2, const vec<ty, 4> &v3, const vec<ty, 4> &v4)
        {
            mMat[0][0] = v1.x;
            mMat[0][1] = v1.y;
            mMat[0][2] = v1.z;
            mMat[0][3] = v1.w;

            mMat[1][0] = v2.x;
            mMat[1][1] = v2.y;
            mMat[1][2] = v2.z;
            mMat[1][3] = v2.w;

            mMat[2][0] = v3.x;
            mMat[2][1] = v3.y;
            mMat[2][2] = v3.z;
            mMat[2][3] = v3.w;

            mMat[3][0] = v4.x;
            mMat[3][1] = v4.y;
            mMat[3][2] = v4.z;
            mMat[3][3] = v4.w;
        }

        constexpr mat(vec<ty, 4> &&v1, vec<ty, 4> &&v2, vec<ty, 4> &&v3, vec<ty, 4> &&v4)
        {
           mMat[0][0] = v1.x;
            mMat[0][1] = v1.y;
            mMat[0][2] = v1.z;
            mMat[0][3] = v1.w;

            mMat[1][0] = v2.x;
            mMat[1][1] = v2.y;
            mMat[1][2] = v2.z;
            mMat[1][3] = v2.w;

            mMat[2][0] = v3.x;
            mMat[2][1] = v3.y;
            mMat[2][2] = v3.z;
            mMat[2][3] = v3.w;

            mMat[3][0] = v4.x;
            mMat[3][1] = v4.y;
            mMat[3][2] = v4.z;
            mMat[3][3] = v4.w;
        }

        constexpr mat(const mat &other)
        {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    mMat[i][j] = other.mMat[i][j];
        }

        constexpr mat(mat &&other)
        {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    mMat[i][j] = std::move(other.mMat[i][j]);
        }

        constexpr mat &operator=(const mat &other)
        {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    mMat[i][j] = other.mMat[i][j];
            return *this;
        }

        constexpr mat &operator=(mat &&other)
        {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    mMat[i][j] = other.mMat[i][j];
            return *this;
        }

        constexpr ty *operator[](unsigned int index)
        {
            return mMat[index];
        }

        constexpr const ty *operator[](unsigned int index) const
        {
            return mMat[index];
        }
    };

    typedef mat<float, 4, 4> mat4;
    typedef mat<int, 4, 4> imat4;
    typedef mat<double, 4, 4> dmat4;

    template <typename ty>
    constexpr mat<ty, 4, 4> operator+(const mat<ty, 4, 4> &lhs, const mat<ty, 4, 4> &rhs)
    {
        mat<ty, 4, 4> result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] + rhs.mMat[i][j];
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> operator+(const mat<ty, 4, 4> &lhs, mat<ty, 4, 4> &&rhs)
    {
        mat<ty, 4, 4> result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] + rhs.mMat[i][j];
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> operator-(const mat<ty, 4, 4> &lhs, const mat<ty, 4, 4> &rhs)
    {
        mat<ty, 4, 4> result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] - rhs.mMat[i][j];
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> operator-(const mat<ty, 4, 4> &lhs, mat<ty, 4, 4> &&rhs)
    {
        mat<ty, 4, 4> result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] - rhs.mMat[i][j];
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> &operator+=(mat<ty, 4, 4> &lhs, const mat<ty, 4, 4> &rhs)
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                lhs.mMat[i][j] += rhs.mMat[i][j];
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> &operator+=(mat<ty, 4, 4> &lhs, mat<ty, 4, 4> &&rhs)
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                lhs.mMat[i][j] += rhs.mMat[i][j];
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> &operator-=(mat<ty, 4, 4> &lhs, const mat<ty, 4, 4> &rhs)
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                lhs.mMat[i][j] -= rhs.mMat[i][j];
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> &operator-=(mat<ty, 4, 4> &lhs, mat<ty, 4, 4> &&rhs)
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                lhs.mMat[i][j] -= rhs.mMat[i][j];
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> operator*(const mat<ty, 4, 4> &lhs, ty scalar)
    {
        mat<ty, 4, 4> result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] * scalar;
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> &operator*=(mat<ty, 4, 4> &lhs, ty scalar)
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                lhs.mMat[i][j] *= scalar;
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> operator/(const mat<ty, 4, 4> &lhs, ty scalar)
    {
        mat<ty, 4, 4> result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] / scalar;
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> &operator/=(mat<ty, 4, 4> &lhs, ty scalar)
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                lhs.mMat[i][j] /= scalar;
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> operator*(const mat<ty, 4, 4> &lhs, const mat<ty, 4, 4> &rhs)
    {
        mat<ty, 4, 4> result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.mMat[i][j] = lhs.mMat[i][0] * rhs.mMat[0][j] +
                                    lhs.mMat[i][1] * rhs.mMat[1][j] +
                                    lhs.mMat[i][2] * rhs.mMat[2][j] +
                                    lhs.mMat[i][3] * rhs.mMat[3][j];
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> operator*(const mat<ty, 4, 4> &lhs, mat<ty, 4, 4> &&rhs)
    {
        mat<ty, 4, 4> result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.mMat[i][j] = lhs.mMat[i][0] * rhs.mMat[0][j] +
                                    lhs.mMat[i][1] * rhs.mMat[1][j] +
                                    lhs.mMat[i][2] * rhs.mMat[2][j] +
                                    lhs.mMat[i][3] * rhs.mMat[3][j];
        return result;
    }

    template <typename ty>
    constexpr vec<ty, 4> operator*(const mat<ty, 4, 4> &lhs, const vec<ty, 4> &rhs)
    {
        vec<ty, 4> result;
        result.x = lhs.mMat[0][0] * rhs.x + lhs.mMat[0][1] * rhs.y + lhs.mMat[0][2] * rhs.z + lhs.mMat[0][3] * rhs.w;
        result.y = lhs.mMat[1][0] * rhs.x + lhs.mMat[1][1] * rhs.y + lhs.mMat[1][2] * rhs.z + lhs.mMat[1][3] * rhs.w;
        result.z = lhs.mMat[2][0] * rhs.x + lhs.mMat[2][1] * rhs.y + lhs.mMat[2][2] * rhs.z + lhs.mMat[2][3] * rhs.w;
        result.w = lhs.mMat[3][0] * rhs.x + lhs.mMat[3][1] * rhs.y + lhs.mMat[3][2] * rhs.z + lhs.mMat[3][3] * rhs.w;
        return result;
    }

    template <typename ty>
    constexpr vec<ty, 4> operator*(const mat<ty, 4, 4> &lhs, vec<ty, 4> &&rhs)
    {
        vec<ty, 4> result;
        result.x = lhs.mMat[0][0] * rhs.x + lhs.mMat[0][1] * rhs.y + lhs.mMat[0][2] * rhs.z + lhs.mMat[0][3] * rhs.w;
        result.y = lhs.mMat[1][0] * rhs.x + lhs.mMat[1][1] * rhs.y + lhs.mMat[1][2] * rhs.z + lhs.mMat[1][3] * rhs.w;
        result.z = lhs.mMat[2][0] * rhs.x + lhs.mMat[2][1] * rhs.y + lhs.mMat[2][2] * rhs.z + lhs.mMat[2][3] * rhs.w;
        result.w = lhs.mMat[3][0] * rhs.x + lhs.mMat[3][1] * rhs.y + lhs.mMat[3][2] * rhs.z + lhs.mMat[3][3] * rhs.w;
        return result;
    }

    template <typename ty>
    constexpr vec<ty, 4> &operator*=(vec<ty, 4> &rhs, const mat<ty, 4, 4> &lhs)
    {
        return rhs = lhs * rhs;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> &operator*=(mat<ty, 4, 4> &lhs, const mat<ty, 4, 4> &rhs)
    {
        return lhs = lhs * rhs;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> &operator*=(mat<ty, 4, 4> &lhs, mat<ty, 4, 4> &&rhs)
    {
        return lhs = lhs * rhs;
    }

    template <typename ty>
    constexpr ty determinant(const mat<ty, 4, 4> &m)
    {
        // Calculate the determinant using cofactor expansion along the first row
        ty a = m.mMat[0][0];
        ty b = m.mMat[0][1];
        ty c = m.mMat[0][2];
        ty d = m.mMat[0][3];

        // Calculate the 3x3 minors
        ty minor00 = m.mMat[1][1] * (m.mMat[2][2] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][2]) -
                     m.mMat[1][2] * (m.mMat[2][1] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][1]) +
                     m.mMat[1][3] * (m.mMat[2][1] * m.mMat[3][2] - m.mMat[2][2] * m.mMat[3][1]);

        ty minor01 = m.mMat[1][0] * (m.mMat[2][2] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][2]) -
                     m.mMat[1][2] * (m.mMat[2][0] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][0]) +
                     m.mMat[1][3] * (m.mMat[2][0] * m.mMat[3][2] - m.mMat[2][2] * m.mMat[3][0]);

        ty minor02 = m.mMat[1][0] * (m.mMat[2][1] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][1]) -
                     m.mMat[1][1] * (m.mMat[2][0] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][0]) +
                     m.mMat[1][3] * (m.mMat[2][0] * m.mMat[3][1] - m.mMat[2][1] * m.mMat[3][0]);

        ty minor03 = m.mMat[1][0] * (m.mMat[2][1] * m.mMat[3][2] - m.mMat[2][2] * m.mMat[3][1]) -
                     m.mMat[1][1] * (m.mMat[2][0] * m.mMat[3][2] - m.mMat[2][2] * m.mMat[3][0]) +
                     m.mMat[1][2] * (m.mMat[2][0] * m.mMat[3][1] - m.mMat[2][1] * m.mMat[3][0]);

        // Apply the cofactor expansion
        return a * minor00 - b * minor01 + c * minor02 - d * minor03;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> transpose(const mat<ty, 4, 4> &m)
    {
        mat<ty, 4, 4> result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.mMat[i][j] = m.mMat[j][i];
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 4, 4> inverse(const mat<ty, 4, 4> &m)
    {
        ty cofactor00 = m.mMat[1][1] * (m.mMat[2][2] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][2]) -
                        m.mMat[1][2] * (m.mMat[2][1] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][1]) +
                        m.mMat[1][3] * (m.mMat[2][1] * m.mMat[3][2] - m.mMat[2][2] * m.mMat[3][1]);

        ty cofactor01 = -(m.mMat[1][0] * (m.mMat[2][2] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][2]) -
                          m.mMat[1][2] * (m.mMat[2][0] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][0]) +
                          m.mMat[1][3] * (m.mMat[2][0] * m.mMat[3][2] - m.mMat[2][2] * m.mMat[3][0]));

        ty cofactor02 = m.mMat[1][0] * (m.mMat[2][1] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][1]) -
                        m.mMat[1][1] * (m.mMat[2][0] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][0]) +
                        m.mMat[1][3] * (m.mMat[2][0] * m.mMat[3][1] - m.mMat[2][1] * m.mMat[3][0]);

        ty cofactor03 = -(m.mMat[1][0] * (m.mMat[2][1] * m.mMat[3][2] - m.mMat[2][2] * m.mMat[3][1]) -
                          m.mMat[1][1] * (m.mMat[2][0] * m.mMat[3][2] - m.mMat[2][2] * m.mMat[3][0]) +
                          m.mMat[1][2] * (m.mMat[2][0] * m.mMat[3][1] - m.mMat[2][1] * m.mMat[3][0]));

        ty cofactor10 = -(m.mMat[0][1] * (m.mMat[2][2] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][2]) -
                          m.mMat[0][2] * (m.mMat[2][1] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][1]) +
                          m.mMat[0][3] * (m.mMat[2][1] * m.mMat[3][2] - m.mMat[2][2] * m.mMat[3][1]));

        ty cofactor11 = m.mMat[0][0] * (m.mMat[2][2] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][2]) -
                        m.mMat[0][2] * (m.mMat[2][0] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][0]) +
                        m.mMat[0][3] * (m.mMat[2][0] * m.mMat[3][2] - m.mMat[2][2] * m.mMat[3][0]);

        ty cofactor12 = -(m.mMat[0][0] * (m.mMat[2][1] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][1]) -
                          m.mMat[0][1] * (m.mMat[2][0] * m.mMat[3][3] - m.mMat[2][3] * m.mMat[3][0]) +
                          m.mMat[0][3] * (m.mMat[2][0] * m.mMat[3][1] - m.mMat[2][1] * m.mMat[3][0]));

        ty cofactor13 = m.mMat[0][0] * (m.mMat[2][1] * m.mMat[3][2] - m.mMat[2][2] * m.mMat[3][1]) -
                        m.mMat[0][1] * (m.mMat[2][0] * m.mMat[3][2] - m.mMat[2][2] * m.mMat[3][0]) +
                        m.mMat[0][2] * (m.mMat[2][0] * m.mMat[3][1] - m.mMat[2][1] * m.mMat[3][0]);

        ty cofactor20 = m.mMat[0][1] * (m.mMat[1][2] * m.mMat[3][3] - m.mMat[1][3] * m.mMat[3][2]) -
                        m.mMat[0][2] * (m.mMat[1][1] * m.mMat[3][3] - m.mMat[1][3] * m.mMat[3][1]) +
                        m.mMat[0][3] * (m.mMat[1][1] * m.mMat[3][2] - m.mMat[1][2] * m.mMat[3][1]);

        ty cofactor21 = -(m.mMat[0][0] * (m.mMat[1][2] * m.mMat[3][3] - m.mMat[1][3] * m.mMat[3][2]) -
                          m.mMat[0][2] * (m.mMat[1][0] * m.mMat[3][3] - m.mMat[1][3] * m.mMat[3][0]) +
                          m.mMat[0][3] * (m.mMat[1][0] * m.mMat[3][2] - m.mMat[1][2] * m.mMat[3][0]));

        ty cofactor22 = m.mMat[0][0] * (m.mMat[1][1] * m.mMat[3][3] - m.mMat[1][3] * m.mMat[3][1]) -
                        m.mMat[0][1] * (m.mMat[1][0] * m.mMat[3][3] - m.mMat[1][3] * m.mMat[3][0]) +
                        m.mMat[0][3] * (m.mMat[1][0] * m.mMat[3][1] - m.mMat[1][1] * m.mMat[3][0]);

        ty cofactor23 = -(m.mMat[0][0] * (m.mMat[1][1] * m.mMat[3][2] - m.mMat[1][2] * m.mMat[3][1]) -
                          m.mMat[0][1] * (m.mMat[1][0] * m.mMat[3][2] - m.mMat[1][2] * m.mMat[3][0]) +
                          m.mMat[0][2] * (m.mMat[1][0] * m.mMat[3][1] - m.mMat[1][1] * m.mMat[3][0]));

        ty cofactor30 = -(m.mMat[0][1] * (m.mMat[1][2] * m.mMat[2][3] - m.mMat[1][3] * m.mMat[2][2]) -
                          m.mMat[0][2] * (m.mMat[1][1] * m.mMat[2][3] - m.mMat[1][3] * m.mMat[2][1]) +
                          m.mMat[0][3] * (m.mMat[1][1] * m.mMat[2][2] - m.mMat[1][2] * m.mMat[2][1]));

        ty cofactor31 = m.mMat[0][0] * (m.mMat[1][2] * m.mMat[2][3] - m.mMat[1][3] * m.mMat[2][2]) -
                        m.mMat[0][2] * (m.mMat[1][0] * m.mMat[2][3] - m.mMat[1][3] * m.mMat[2][0]) +
                        m.mMat[0][3] * (m.mMat[1][0] * m.mMat[2][2] - m.mMat[1][2] * m.mMat[2][0]);

        ty cofactor32 = -(m.mMat[0][0] * (m.mMat[1][1] * m.mMat[2][3] - m.mMat[1][3] * m.mMat[2][1]) -
                          m.mMat[0][1] * (m.mMat[1][0] * m.mMat[2][3] - m.mMat[1][3] * m.mMat[2][0]) +
                          m.mMat[0][3] * (m.mMat[1][0] * m.mMat[2][1] - m.mMat[1][1] * m.mMat[2][0]));

        ty cofactor33 = m.mMat[0][0] * (m.mMat[1][1] * m.mMat[2][2] - m.mMat[1][2] * m.mMat[2][1]) -
                        m.mMat[0][1] * (m.mMat[1][0] * m.mMat[2][2] - m.mMat[1][2] * m.mMat[2][0]) +
                        m.mMat[0][2] * (m.mMat[1][0] * m.mMat[2][1] - m.mMat[1][1] * m.mMat[2][0]);

        
        ty det = m.mMat[0][0] * cofactor00 + m.mMat[0][1] * cofactor01 +
                 m.mMat[0][2] * cofactor02 + m.mMat[0][3] * cofactor03;

        if (det == static_cast<ty>(0))
            return mat<ty, 4, 4>(ty(1)); 

       
        ty invDet = static_cast<ty>(1) / det;

        mat<ty, 4, 4> result;
        result.mMat[0][0] = cofactor00 * invDet;
        result.mMat[0][1] = cofactor10 * invDet;
        result.mMat[0][2] = cofactor20 * invDet;
        result.mMat[0][3] = cofactor30 * invDet;
        result.mMat[1][0] = cofactor01 * invDet;
        result.mMat[1][1] = cofactor11 * invDet;
        result.mMat[1][2] = cofactor21 * invDet;
        result.mMat[1][3] = cofactor31 * invDet;
        result.mMat[2][0] = cofactor02 * invDet;
        result.mMat[2][1] = cofactor12 * invDet;
        result.mMat[2][2] = cofactor22 * invDet;
        result.mMat[2][3] = cofactor32 * invDet;
        result.mMat[3][0] = cofactor03 * invDet;
        result.mMat[3][1] = cofactor13 * invDet;
        result.mMat[3][2] = cofactor23 * invDet;
        result.mMat[3][3] = cofactor33 * invDet;

        return result;
    }
}

#endif // __MAT4_H__