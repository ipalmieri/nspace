#ifndef __VECTOR4_H__
#define __VECTOR4_H__

#include <cmath>
#include <cassert>
#include "adefs.h"
#include "vector3.h"
#include "matrix4.h"

namespace tools {

//////////////////////////////////////////////////////
/// euclidean fourth-dimensional vector representation
//////////////////////////////////////////////////////
class vector4
{
 public:

  static const vector4 ZERO;

  Real x, y, z, w;

  vector4() {}

  vector4(const Real& a1, const Real& a2, const Real& a3, const Real& a4)
    : x(a1),
      y(a2),
      z(a3),
      w(a4) {}
  vector4(const Real vec[4]): x(vec[0]), y(vec[1]), z(vec[2]), w(vec[3]) {}
  vector4(const vector3& v, const Real& nw=1.0f): x(v[0]), y(v[1]), z(v[2]),
    w(nw) {}
  ~vector4() {}

  inline Real operator[](const int& i) const
  {
    assert(i >= 0 && i < 4);

    if (i == 0) {
      return x;
    }
    if (i == 1) {
      return y;
    }
    if (i == 2) {
      return z;
    }
    /*if (i == 3)*/ return w;

  }

  inline Real& operator[](const int& i)
  {
    assert(i >= 0 && i < 4);

    if (i == 0) {
      return *(&x);
    }
    if (i == 1) {
      return *(&y);
    }
    if (i == 2) {
      return *(&z);
    }
    /*if (i == 3)*/ return *(&w);

  }

  inline vector4& operator=(const vector4& vec)
  {
    x = vec.x;
    y = vec.y;
    z = vec.z;
    w = vec.w;

    return *this;
  }

  inline vector4 operator+(const vector4& vec) const
  {
    return vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
  }

  inline vector4 operator-(const vector4& vec) const
  {
    return vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
  }

  inline vector4 operator*(const Real& scalar) const
  {
    return vector4(scalar*x, scalar*y, scalar*z, scalar*w);
  }


  inline vector4 operator/(const Real& scalar) const
  {
    return vector4(x/scalar, y/scalar, z/scalar, w/scalar);
  }

  inline vector4 operator+() const
  {
    return *this;
  }

  inline vector4 operator-() const
  {
    return vector4(-x, -y, -z, -w);
  }

  inline vector4& operator+=(const vector4& vec)
  {
    x += vec.x;
    y += vec.y;
    z += vec.z;
    w += vec.w;

    return *this;
  }

  inline vector4& operator-=(const vector4& vec)
  {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    w -= vec.w;

    return *this;
  }

  inline vector4& operator*=(const Real& scalar)
  {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;

    return *this;
  }

  inline vector4& operator/=(const Real& scalar)
  {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;

    return *this;
  }

  //dot product
  inline Real operator*(const vector4& vec) const
  {
    return x*vec.x + y*vec.y + z*vec.z + w*vec.w;
  }

  //operator* por matrix4;
  vector4 operator *(const matrix4& mat) const;

 protected:

};

Real length(const vector4& vec);
vector4 normalize(const vector4& vec);

} //__VECTOR4_H__

#endif
