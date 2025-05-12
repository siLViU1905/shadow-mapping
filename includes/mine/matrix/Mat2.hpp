#ifndef __MAT2_H__
#define __MAT2_H__
#include "Mat.hpp"
#include "../vector/Vec2.hpp"

namespace sml
{
    template <typename ty>
    class mat<ty, 2, 2>
    {
    public:
        ty mMat[2][2];

        constexpr mat()
        {
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 2; ++j)
                    mMat[i][j] = ty(0);
        }

        constexpr mat(ty scalar)
        {
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 2; ++j)
                    if (i == j)
                        mMat[i][j] = scalar;
                    else
                        mMat[i][j] = ty(0);
        }

        constexpr mat(const mat &other)
        {
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 2; ++j)
                    mMat[i][j] = other.mMat[i][j];
        }

        constexpr mat(mat &&other)
        {
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 2; ++j)
                    mMat[i][j] = std::move(other.mMat[i][j]);
        }

        constexpr mat(const vec<ty, 2> &v1, const vec<ty, 2> &v2)
        {
            mMat[0][0] = v1.x;
            mMat[0][1] = v1.y;

            mMat[1][0] = v2.x;
            mMat[1][1] = v2.y;
        }

        constexpr mat(vec<ty, 2> &&v1, vec<ty, 2> &&v2)
        {
            mMat[0][0] = std::move(v1.x);
            mMat[0][1] = std::move(v1.y);

            mMat[1][0] = std::move(v2.x);
            mMat[1][1] = std::move(v2.y);
        }

        constexpr mat &operator=(const mat &other)
        {
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 2; ++j)
                    mMat[i][j] = other.mMat[i][j];
            return *this;
        }

        constexpr mat &operator=(mat &&other)
        {
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 2; ++j)
                    mMat[i][j] = std::move(other.mMat[i][j]);
            return *this;
        }

        constexpr ty* operator[](unsigned int index)
        {
            return mMat[index];
        }

        constexpr const ty* operator[](unsigned int index) const
        {
            return mMat[index];
        }
    };

    typedef mat<float, 2, 2> mat2;
    typedef mat<int, 2, 2> imat2;
    typedef mat<double, 2, 2> dmat2;

    template <typename ty>
    constexpr mat<ty, 2, 2> operator+(const mat<ty, 2, 2> &lhs, const mat<ty, 2, 2> &rhs)
    {
        mat<ty, 2, 2> result;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] + rhs.mMat[i][j];
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 2, 2> operator+(const mat<ty, 2, 2> &lhs, mat<ty, 2, 2> &&rhs)
    {
        mat<ty, 2, 2> result;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] + rhs.mMat[i][j];
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 2, 2> operator-(const mat<ty, 2, 2> &lhs, const mat<ty, 2, 2> &rhs)
    {
        mat<ty, 2, 2> result;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] - rhs.mMat[i][j];
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 2, 2> operator-(const mat<ty, 2, 2> &lhs, mat<ty, 2, 2> &&rhs)
    {
        mat<ty, 2, 2> result;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] - rhs.mMat[i][j];
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 2, 2> &operator+=(mat<ty, 2, 2> &lhs, const mat<ty, 2, 2> &rhs)
    {
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                lhs.mMat[i][j] += rhs.mMat[i][j];
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 2, 2> &operator+=(mat<ty, 2, 2> &lhs, mat<ty, 2, 2> &&rhs)
    {
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                lhs.mMat[i][j] += rhs.mMat[i][j];
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 2, 2> &operator-=(mat<ty, 2, 2> &lhs, const mat<ty, 2, 2> &rhs)
    {
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                lhs.mMat[i][j] -= rhs.mMat[i][j];
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 2, 2> &operator-=(mat<ty, 2, 2> &lhs, mat<ty, 2, 2> &&rhs)
    {
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                lhs.mMat[i][j] -= rhs.mMat[i][j];
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 2, 2> operator*(const mat<ty, 2, 2> &lhs, ty scalar)
    {
        mat<ty, 2, 2> result;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] * scalar;
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 2, 2> &operator*=(mat<ty, 2, 2> &lhs, ty scalar)
    {
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                lhs.mMat[i][j] *= scalar;
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 2, 2> operator/(const mat<ty, 2, 2> &lhs, ty scalar)
    {
        mat<ty, 2, 2> result;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                result.mMat[i][j] = lhs.mMat[i][j] / scalar;
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 2, 2> &operator/=(mat<ty, 2, 2> &lhs, ty scalar)
    {
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                lhs.mMat[i][j] /= scalar;
        return lhs;
    }

    template <typename ty>
    constexpr mat<ty, 2, 2> operator*(const mat<ty, 2, 2> &lhs, const mat<ty, 2, 2> &rhs)
    {
        mat<ty, 2, 2> result;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                result.mMat[i][j] = lhs.mMat[i][0] * rhs.mMat[0][j] + lhs.mMat[i][1] * rhs.mMat[1][j];
        return result;
    }

    template <typename ty>
    constexpr mat<ty, 2, 2> operator*(const mat<ty, 2, 2> &lhs, mat<ty, 2, 2> &&rhs)
    {
        mat<ty, 2, 2> result;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                result.mMat[i][j] = lhs.mMat[i][0] * rhs.mMat[0][j] + lhs.mMat[i][1] * rhs.mMat[1][j];
        return result;
    }

    template <typename ty>
    constexpr vec<ty, 2> operator*(const mat<ty, 2, 2> &lhs, const vec<ty, 2> &rhs)
    {
       vec<ty, 2> result;
       result.x = lhs.mMat[0][0] * rhs.x + lhs.mMat[0][1] * rhs.y; 
       result.y = lhs.mMat[1][0] * rhs.x + lhs.mMat[1][1] * rhs.y; 
        return result;
    }

     template <typename ty>
    constexpr vec<ty, 2> operator*(const mat<ty, 2, 2> &lhs, vec<ty, 2> &&rhs)
    {
       vec<ty, 2> result;
       result.x = lhs.mMat[0][0] * rhs.x + lhs.mMat[0][1] * rhs.y; 
       result.y = lhs.mMat[1][0] * rhs.x + lhs.mMat[1][1] * rhs.y; 
        return result;
    }

    template <typename ty>
    constexpr vec<ty, 2>& operator*=(const mat<ty, 2, 2> &lhs, vec<ty, 2> &rhs)
    {
       return rhs = lhs * rhs;
    }

    template <typename ty>
    constexpr mat<ty, 2, 2> &operator*=(mat<ty, 2, 2> &lhs, const mat<ty, 2, 2> &rhs)
    {
        return lhs = lhs * rhs;
    }

    template <typename ty>
    constexpr mat<ty, 2, 2> &operator*=(mat<ty, 2, 2> &lhs, mat<ty, 2, 2> &&rhs)
    {
        return lhs = lhs * rhs;
    }

    template <typename ty>
    constexpr ty determinant(const mat<ty, 2, 2> &m)
    {
        return m.mMat[0][0] * m.mMat[1][1] - m.mMat[0][1] * m.mMat[1][0];
    }

    template <typename ty>
    constexpr mat<ty, 2, 2> transpose(const mat<ty, 2, 2> &m)
    {
        mat<ty, 2, 2> result;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                result[i][j] = m[j][i];
    }

    template <typename ty>
    constexpr mat<ty, 2, 2> inverse(const mat<ty, 2, 2> &m)
    {
        return mat<ty, 2, 2>(vec<ty, 2>(m.mMat[1][1], -m.mMat[0][1]),
        vec<ty, 2>(vec<ty, 2>(-m.mMat[1][0], -m.mMat[0][0]))) * (ty(1) / determinant(m));
    }
}

#endif // __MAT2_H__