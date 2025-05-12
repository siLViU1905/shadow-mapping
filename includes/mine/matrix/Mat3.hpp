#ifndef __MAT3_H__
#define __MAT3_H__
#include "Mat.hpp"

namespace sml
{
    template <typename ty>
    class mat<ty, 3, 3>
    {
    public:
        ty mMat[3][3];

        constexpr mat()
        {
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    mMat[i][j] = ty(0);
        }

        constexpr mat(ty scalar)
        {
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    if (i == j)
                        mMat[i][j] = scalar;
                    else
                        mMat[i][j] = ty(0);
        }

        constexpr mat(const vec<ty, 3> &v1, const vec<ty, 3> &v2, const vec<ty, 3> &v3)
        {
            mMat[0][0] = v1.x;
            mMat[0][1] = v1.y;
            mMat[0][2] = v1.z;

            mMat[1][0] = v2.x;
            mMat[1][1] = v2.y;
            mMat[1][2] = v2.z;

            mMat[2][0] = v3.x;
            mMat[2][1] = v3.y;
            mMat[2][2] = v3.z;
        }

        constexpr mat(vec<ty, 3> &&v1, vec<ty, 3> &&v2, vec<ty, 3> &&v3)
        {
            mMat[0][0] = std::move(v1.x);
            mMat[0][1] = std::move(v1.y);
            mMat[0][2] = std::move(v1.z);

            mMat[1][0] = std::move(v2.x);
            mMat[1][1] = std::move(v2.y);
            mMat[1][2] = std::move(v2.z);

            mMat[2][0] = std::move(v3.x);
            mMat[2][1] = std::move(v3.y);
            mMat[2][2] = std::move(v3.z);
        }

        constexpr mat(const mat &other)
        {
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    mMat[i][j] = other.mMat[i][j];
        }

        constexpr mat(mat &&other)
        {
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    mMat[i][j] = std::move(other.mMat[i][j]);
        }

        constexpr mat &operator=(const mat &other)
        {
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    mMat[i][j] = other.mMat[i][j];
            return *this;
        }

        constexpr mat &operator=(mat &&other)
        {
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    mMat[i][j] = std::move(other.mMat[i][j]);
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

    typedef mat<float, 3, 3> mat3;
    typedef mat<int, 3, 3> imat3;
    typedef mat<double, 3, 3> dmat3;

    template <typename ty>
    constexpr mat<ty, 3, 3> operator+(const mat<ty, 3, 3> &lhs, const mat<ty, 3, 3> &rhs)
    {
        mat<ty, 3, 3> result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] + rhs.mMat[i][j];
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 3, 3> operator+(const mat<ty, 3, 3> &lhs, mat<ty, 3, 3> &&rhs)
    {
        mat<ty, 3, 3> result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] + rhs.mMat[i][j];
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 3, 3> operator-(const mat<ty, 3, 3> &lhs, const mat<ty, 3, 3> &rhs)
    {
        mat<ty, 3, 3> result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] - rhs.mMat[i][j];
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 3, 3> operator-(const mat<ty, 3, 3> &lhs, mat<ty, 3, 3> &&rhs)
    {
        mat<ty, 3, 3> result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] - rhs.mMat[i][j];
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 3, 3> &operator+=(mat<ty, 3, 3> &lhs, const mat<ty, 3, 3> &rhs)
    {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                lhs.mMat[i][j] += rhs.mMat[i][j];
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 3, 3> &operator+=(mat<ty, 3, 3> &lhs, mat<ty, 3, 3> &&rhs)
    {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                lhs.mMat[i][j] += rhs.mMat[i][j];
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 3, 3> &operator-=(mat<ty, 3, 3> &lhs, const mat<ty, 3, 3> &rhs)
    {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                lhs.mMat[i][j] -= rhs.mMat[i][j];
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 3, 3> &operator-=(mat<ty, 3, 3> &lhs, mat<ty, 3, 3> &&rhs)
    {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                lhs.mMat[i][j] -= rhs.mMat[i][j];
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 3, 3> operator*(const mat<ty, 3, 3> &lhs, ty scalar)
    {
        mat<ty, 3, 3> result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] * scalar;
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 3, 3> &operator*=(mat<ty, 3, 3> &lhs, ty scalar)
    {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                lhs.mMat[i][j] *= scalar;
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 3, 3> operator/(const mat<ty, 3, 3> &lhs, ty scalar)
    {
        mat<ty, 3, 3> result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] / scalar;
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 3, 3> &operator/=(mat<ty, 3, 3> &lhs, ty scalar)
    {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                lhs.mMat[i][j] /= scalar;
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 3, 3> operator*(const mat<ty, 3, 3> &lhs, const mat<ty, 3, 3> &rhs)
    {
        mat<ty, 3, 3> result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result.mMat[i][j] = lhs.mMat[i][0] * rhs.mMat[0][j] +
                                    lhs.mMat[i][1] * rhs.mMat[1][j] +
                                    lhs.mMat[i][2] * rhs.mMat[2][j];
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 3, 3> operator*(const mat<ty, 3, 3> &lhs, mat<ty, 3, 3> &&rhs)
    {
        mat<ty, 3, 3> result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result.mMat[i][j] = lhs.mMat[i][0] * rhs.mMat[0][j] +
                                    lhs.mMat[i][1] * rhs.mMat[1][j] +
                                    lhs.mMat[i][2] * rhs.mMat[2][j];
        return result;
    }

    template <typename ty>
    constexpr vec<ty, 3> operator*(const mat<ty, 3, 3> &lhs, const vec<ty, 3> &rhs)
    {
        vec<ty, 3> result;
        result.x = lhs.mMat[0][0] * rhs.x + lhs.mMat[0][1] * rhs.y + lhs.mMat[0][2] * rhs.z;
        result.y = lhs.mMat[1][0] * rhs.x + lhs.mMat[1][1] * rhs.y + lhs.mMat[1][2] * rhs.z;
        result.z = lhs.mMat[2][0] * rhs.x + lhs.mMat[2][1] * rhs.y + lhs.mMat[2][2] * rhs.z;
        return result;
    }

    template <typename ty>
    constexpr vec<ty, 3> operator*(const mat<ty, 3, 3> &lhs, vec<ty, 3> &&rhs)
    {
        vec<ty, 3> result;
        result.x = lhs.mMat[0][0] * rhs.x + lhs.mMat[0][1] * rhs.y + lhs.mMat[0][2] * rhs.z;
        result.y = lhs.mMat[1][0] * rhs.x + lhs.mMat[1][1] * rhs.y + lhs.mMat[1][2] * rhs.z;
        result.z = lhs.mMat[2][0] * rhs.x + lhs.mMat[2][1] * rhs.y + lhs.mMat[2][2] * rhs.z;
        return result;
    }

    template <typename ty>
    constexpr vec<ty, 3> &operator*=(const mat<ty, 3, 3> &lhs, vec<ty, 3> &rhs)
    {
        return rhs = lhs * rhs;
    }

    template <typename ty>
    constexpr mat<ty, 3, 3> &operator*=(mat<ty, 3, 3> &lhs, const mat<ty, 3, 3> &rhs)
    {
        return lhs = lhs * rhs;
    }

    template <typename ty>
    constexpr mat<ty, 3, 3> &operator*=(mat<ty, 3, 3> &lhs, mat<ty, 3, 3> &&rhs)
    {
        return lhs = lhs * rhs;
    }

    template <typename ty>
    constexpr ty determinant(const mat<ty, 3, 3> &m)
    {
        return m.mMat[0][0] * (m.mMat[1][1] * m.mMat[2][2] - m.mMat[1][2] * m.mMat[2][1]) - m.mMat[0][1] * (m.mMat[1][0] * m.mMat[2][2] - m.mMat[1][2] * m.mMat[2][0]) + m.mMat[0][2] * (m.mMat[1][0] * m.mMat[2][1] - m.mMat[1][1] * m.mMat[2][0]);
    }

    template <typename ty>
    constexpr mat<ty, 3, 3> transpose(const mat<ty, 3, 3> &m)
    {
        mat<ty, 3, 3> result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result[i][j] = m[j][i];
    }

    template <typename ty>
    constexpr mat<ty, 3, 3> inverse(const mat<ty, 3, 3> &m)
    {

        const ty &a = m.mMat[0][0];
        const ty &b = m.mMat[0][1];
        const ty &c = m.mMat[0][2];
        const ty &d = m.mMat[1][0];
        const ty &e = m.mMat[1][1];
        const ty &f = m.mMat[1][2];
        const ty &g = m.mMat[2][0];
        const ty &h = m.mMat[2][1];
        const ty &i = m.mMat[2][2];

        ty ei_fh = e * i - f * h;
        ty fg_di = f * g - d * i;
        ty dh_eg = d * h - e * g;

        ty det = a * ei_fh + b * fg_di + c * dh_eg;

        if (det == static_cast<ty>(0))
            return mat<ty, 3, 3>(ty(1));

        ty invDet = static_cast<ty>(1) / det;

        ty bi_ch = b * i - c * h;
        ty cg_ai = c * g - a * i;
        ty ah_bg = a * h - b * g;
        ty bf_ce = b * f - c * e;
        ty cd_af = c * d - a * f;
        ty ae_bd = a * e - b * d;

        return mat<ty, 3, 3>(
            vec<ty, 3>(ei_fh * invDet, bi_ch * invDet, bf_ce * invDet),
            vec<ty, 3>(fg_di * invDet, cg_ai * invDet, cd_af * invDet),
            vec<ty, 3>(dh_eg * invDet, ah_bg * invDet, ae_bd * invDet));
    }
}
#endif // __MAT3_H__