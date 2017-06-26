#include "algebra/matrix3.h"
#include "algebra/vector3.h"

using namespace std;
using namespace tools;

const matrix3 matrix3::ZERO(0, 0, 0,
                            0, 0, 0,
                            0, 0, 0);

const matrix3 matrix3::IDENTITY(1, 0, 0,
                                0, 1, 0,
                                0, 0, 1);

matrix3 matrix3::operator+(const matrix3& mat) const
{
  matrix3 sum;

  for (unsigned i = 0; i < 3; i++) {
    for (unsigned j = 0; j < 3; j++) {
      sum[i][j] = _matrix[i][j] + mat[i][j];
    }
  }

  return sum;
}

matrix3 matrix3::operator-(const matrix3& mat) const
{
  matrix3 diff;

  for (unsigned i = 0; i < 3; i++) {
    for (unsigned j = 0; j < 3; j++) {
      diff[i][j] = _matrix[i][j] - mat[i][j];
    }
  }

  return diff;
}

matrix3 matrix3::operator*(const matrix3& mat) const
{
  matrix3 prod;

  for (unsigned i = 0; i < 3; i++) {
    for (unsigned j = 0; j < 3; j++) {
      prod[i][j] = 0.0;

      for (unsigned k = 0; k < 3; k++) {
        prod[i][j] += _matrix[i][k]*mat[k][j];
      }

    }
  }

  return prod;
}

matrix3 matrix3::operator*(const Real& scalar) const
{
  matrix3 prod;

  for (unsigned i = 0; i < 3; i++) {
    for (unsigned j = 0; j < 3; j++) {
      prod[i][j] = _matrix[i][j]*scalar;
    }
  }
  return prod;
}

vector3 matrix3::operator*(const vector3&  vec) const
{
  vector3 ret;

  ret[0] = vec[0]*_matrix[0][0] + vec[1]*_matrix[0][1] + vec[2]*_matrix[0][2];
  ret[1] = vec[0]*_matrix[1][0] + vec[1]*_matrix[1][1] + vec[2]*_matrix[1][2];
  ret[2] = vec[0]*_matrix[2][0] + vec[1]*_matrix[2][1] + vec[2]*_matrix[2][2];

  return ret;
}

matrix3 tools::operator*(const Real& scalar, const matrix3& mat)
{
  return mat*scalar;
}

matrix3 matrix3::transpose() const
{
  matrix3 mtrans;

  for (unsigned i = 0; i < 3; i++) {
    for (unsigned j = 0; j < 3; j++) {
      mtrans[i][j] = _matrix[j][i];
    }
  }

  return mtrans;
}

matrix3 matrix3::inverse() const
{
  matrix3 inverse;

  inverse[0][0] = _matrix[1][1]*_matrix[2][2] - _matrix[1][2]*_matrix[2][1];
  inverse[0][1] = _matrix[0][2]*_matrix[2][1] - _matrix[0][1]*_matrix[2][2];
  inverse[0][2] = _matrix[0][1]*_matrix[1][2] - _matrix[0][2]*_matrix[1][1];
  inverse[1][0] = _matrix[1][2]*_matrix[2][0] - _matrix[1][0]*_matrix[2][2];
  inverse[1][1] = _matrix[0][0]*_matrix[2][2] - _matrix[0][2]*_matrix[2][0];
  inverse[1][2] = _matrix[0][2]*_matrix[1][0] - _matrix[0][0]*_matrix[1][2];
  inverse[2][0] = _matrix[1][0]*_matrix[2][1] - _matrix[1][1]*_matrix[2][0];
  inverse[2][1] = _matrix[0][1]*_matrix[2][0] - _matrix[0][0]*_matrix[2][1];
  inverse[2][2] = _matrix[0][0]*_matrix[1][1] - _matrix[0][1]*_matrix[1][0];

  Real det = _matrix[0][0]*inverse[0][0] + _matrix[0][1]*inverse[1][0] +
             _matrix[0][2]*inverse[2][0];

  assert(std::abs(det) > 0.0);

  // fix this - probably shouldnt return zero
  if (std::abs(det) == 0.0) {
    return matrix3::ZERO;
  }

  Real idet = 1.0/det;

  for (unsigned i = 0; i < 3; i++) {
    for (unsigned j = 0; j < 3; j++) {
      inverse[i][j] *= idet;
    }
  }

  return inverse;

}

Real matrix3::determinant() const
{
  Real cofactor00 = _matrix[1][1]*_matrix[2][2] - _matrix[1][2]*_matrix[2][1];
  Real cofactor10 = _matrix[1][2]*_matrix[2][0] - _matrix[1][0]*_matrix[2][2];
  Real cofactor20 = _matrix[1][0]*_matrix[2][1] - _matrix[1][1]*_matrix[2][0];

  Real det = _matrix[0][0]*cofactor00 + _matrix[0][1]*cofactor10 +
             _matrix[0][2]*cofactor20;

  return det;

}
