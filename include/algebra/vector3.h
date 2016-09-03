#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <cmath>
#include <cassert>
#include "matrix3.h"

namespace tools {

///////////////////////////////////////////////////
/// euclidean tridimensional vector representation
///////////////////////////////////////////////////
class vector3
{
 public:

  static const vector3 ZERO;
  static const vector3 X;
  static const vector3 Y;
  static const vector3 Z;

  Real x, y, z;

  vector3() {}

  vector3(const Real& a1, const Real& a2, const Real& a3): x(a1), y(a2), z(a3) {}
  vector3(const Real vec[3]): x(vec[0]), y(vec[1]), z(vec[2]) {}
  vector3(const vector3& v): x(v[0]), y(v[1]), z(v[2]) {}
  ~vector3() {}

  inline Real operator [](const int& i) const
  {
    assert(i >= 0 && i < 3);

    if (i == 0) {
      return x;
    }
    if (i == 1) {
      return y;
    }
    //if (i == 2)
    return z;
  }

  inline Real& operator [](const int& i)
  {
    assert(i >= 0 && i < 3);

    if (i == 0) {
      return *(&x);
    }
    if (i == 1) {
      return *(&y);
    }
    //if (i == 2)
    return *(&z);
  }

  inline vector3& operator =(const vector3& vec)
  {
    x = vec.x;
    y = vec.y;
    z = vec.z;

    return *this;
  }

  inline vector3 operator +(const vector3& vec) const
  {
    return vector3(x + vec.x, y + vec.y, z + vec.z);
  }

  inline vector3 operator -(const vector3& vec) const
  {
    return vector3(x - vec.x, y - vec.y, z - vec.z);
  }

  inline vector3 operator *(const Real& scalar) const
  {
    return vector3(scalar*x, scalar*y, scalar*z);
  }


  inline vector3 operator /(const Real& scalar) const
  {
    return vector3(x/scalar, y/scalar, z/scalar);
  }

  inline vector3 operator +() const
  {
    return *this;
  }

  inline vector3 operator -() const
  {
    return vector3(-x, -y, -z);
  }

  inline vector3& operator +=(const vector3& vec)
  {
    x += vec.x;
    y += vec.y;
    z += vec.z;

    return *this;
  }

  inline vector3& operator -=(const vector3& vec)
  {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;

    return *this;
  }

  inline vector3& operator *=(const Real& scalar)
  {
    x *= scalar;
    y *= scalar;
    z *= scalar;

    return *this;
  }

  inline vector3& operator /=(const Real& scalar)
  {
    x /= scalar;
    y /= scalar;
    z /= scalar;

    return *this;
  }

  //dot product
  inline Real operator *(const vector3& vec) const
  {
    return x*vec[0] + y*vec[1] + z*vec[2];
  }

  //cross product
  inline vector3 operator ^(const vector3& vec) const
  {
    return vector3(y*vec[2] - z*vec[1],
                   z*vec[0] - x*vec[2],
                   x*vec[1] - y*vec[0]);
  }

  vector3 operator *(const matrix3& mat) const;

 protected:

};

Real length(const vector3& vec);
vector3 normalize(const vector3& vec);

}

#endif //__VECTOR3_H__
