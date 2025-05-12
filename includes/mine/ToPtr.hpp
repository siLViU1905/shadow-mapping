#ifndef __TOPTR_H__
#define __TOPTR_H__
#include "vector/Vec.hpp"
#include "matrix/Mat.hpp"

namespace sml
{
  template<typename ty>
  constexpr ty* as_ptr(vec<ty, 2>& v)
  {
    return &v.x;
  }

  template<typename ty>
  constexpr const ty* as_ptr(const vec<ty, 2>& v)
  {
    return &v.x;
  }

  template<typename ty>
  constexpr ty* as_ptr(vec<ty, 3>& v)
  {
    return &v.x;
  }

  template<typename ty>
  constexpr const ty* as_ptr(const vec<ty, 3>& v)
  {
    return &v.x;
  }

  template<typename ty>
  constexpr ty* as_ptr(vec<ty, 4>& v)
  {
    return &v.x;
  }

  template<typename ty>
  constexpr const ty* as_ptr(const vec<ty, 4>& v)
  {
    return &v.x;
  }

  template<typename ty>
  constexpr ty* as_ptr(mat<ty, 2, 2>& m)
  {
    return m.mMat[0];
  }

  template<typename ty>
  constexpr const ty* as_ptr(const mat<ty, 2, 2>& m)
  {
    return m.mMat[0];
  }

  template<typename ty>
  constexpr ty* as_ptr(mat<ty, 3, 3>& m)
  {
    return m.mMat[0];
  }

  template<typename ty>
  constexpr const ty* as_ptr(const mat<ty, 3, 3>& m)
  {
    return m.mMat[0];
  }

  template<typename ty>
  constexpr ty* as_ptr(mat<ty, 4, 4>& m)
  {
    return m.mMat[0];
  }

  template<typename ty>
  constexpr const ty* as_ptr(const mat<ty, 4, 4>& m)
  {
    return m.mMat[0];
  }
}

#endif // __TOPTR_H__