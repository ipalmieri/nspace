#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#include "adefs.h"

namespace tools {
class vector4;

//////////////////////////////////////////////
/// 4x4 matrix representation
//////////////////////////////////////////////
class matrix4
{
 public:

  static const matrix4 ZERO;
  static const matrix4 IDENTITY;

  matrix4(Real m[4][4])
  {
    for (unsigned i = 0; i < 4; i++) {
      for (unsigned j = 0; j < 4; j++) {
        _matrix[i][j] = m[i][j];
      }
    }
  }

  matrix4(const matrix4& mat=ZERO)
  {
    for (unsigned i = 0; i < 4; i++) {
      for (unsigned j = 0; j < 4; j++) {
        _matrix[i][j] = mat._matrix[i][j];
      }
    }
  }

  matrix4(Real a00, Real a01, Real a02, Real a03,
          Real a10, Real a11, Real a12, Real a13,
          Real a20, Real a21, Real a22, Real a23,
          Real a30, Real a31, Real a32, Real a33)
  {
    _matrix[0][0] = a00;
    _matrix[0][1] = a01;
    _matrix[0][2] = a02;
    _matrix[0][3] = a03;

    _matrix[1][0] = a10;
    _matrix[1][1] = a11;
    _matrix[1][2] = a12;
    _matrix[1][3] = a13;

    _matrix[2][0] = a20;
    _matrix[2][1] = a21;
    _matrix[2][2] = a22;
    _matrix[2][3] = a23;

    _matrix[3][0] = a30;
    _matrix[3][1] = a31;
    _matrix[3][2] = a32;
    _matrix[3][3] = a33;

  }

  ~matrix4() {};

  Real* operator[](const unsigned& index) const
  {
    return (Real*)_matrix[index];
  }

  matrix4 operator+(const matrix4& mat) const;
  matrix4 operator-(const matrix4& mat) const;
  matrix4 operator*(const matrix4& mat) const;
  matrix4 operator*(const Real& scalar) const;
  vector4 operator*(const vector4& vec) const;
  friend matrix4 operator*(const Real& scalar, const matrix4& mat);

  matrix4 transpose() const;
  matrix4 inverse() const;
  Real determinant() const;

 protected:

  union {
    Real _matrix[4][4];
    Real _mat[16];
  };

};

matrix4 operator*(const Real& scalar, const matrix4& mat);

}

#endif //__MATRIX4_H__
