#ifndef __MATRIX3_H__
#define __MATRIX3_H__

#include "adefs.h"


namespace tools {
class vector3;

//////////////////////////////////////////////
/// 3x3 matrix representation
//////////////////////////////////////////////
class matrix3
{
 public:

  static const matrix3 ZERO;
  static const matrix3 IDENTITY;

  matrix3(Real m[3][3])
  {
    for (unsigned i = 0; i < 3; i++) {
      for (unsigned j = 0; j < 3; j++) {
        _matrix[i][j] = m[i][j];
      }
    }
  }

  matrix3(const matrix3& mat=ZERO)
  {
    for (unsigned i = 0; i < 3; i++) {
      for (unsigned j = 0; j < 3; j++) {
        _matrix[i][j] = mat._matrix[i][j];
      }
    }
  }

  matrix3(Real a00, Real a01, Real a02,
          Real a10, Real a11, Real a12,
          Real a20, Real a21, Real a22)
  {
    _matrix[0][0] = a00;
    _matrix[0][1] = a01;
    _matrix[0][2] = a02;
    _matrix[1][0] = a10;
    _matrix[1][1] = a11;
    _matrix[1][2] = a12;
    _matrix[2][0] = a20;
    _matrix[2][1] = a21;
    _matrix[2][2] = a22;

  }

  ~matrix3() {};

  inline Real* operator[](const unsigned& index) const
  {
    return (Real*)_matrix[index];
  }

  matrix3 operator+(const matrix3& mat) const;
  matrix3 operator-(const matrix3& mat) const;
  matrix3 operator*(const matrix3& mat) const;
  matrix3 operator*(const Real& scalar) const;
  vector3 operator*(const vector3& vec) const;
  
  matrix3 transpose() const;
  matrix3 inverse() const;
  Real determinant() const;

 protected:

  union {
    Real _matrix[3][3];
    Real _mat[9];
  };

};

matrix3 operator*(const Real& scalar, const matrix3& mat);

}

#endif //__MATRIX3_H__
