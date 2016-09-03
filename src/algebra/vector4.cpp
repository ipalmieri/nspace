#include "algebra/vector4.h"

using namespace tools;

const vector4 vector4::ZERO(0, 0, 0, 0);

Real tools::length(const vector4& vec)
{
  return sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2] + vec[3]*vec[3]);
}

vector4 tools::normalize(const vector4& vec)
{
  Real len = length(vec);

  return (len > 0.0)? vec/len : vec;
}

vector4 vector4::operator*(const matrix4& mat) const
{
  vector4 ret;

  ret[0] = x*mat[0][0] + y*mat[1][0] + z*mat[2][0] + w*mat[3][0];
  ret[1] = x*mat[0][1] + y*mat[1][1] + z*mat[2][1] + w*mat[3][1];
  ret[2] = x*mat[0][2] + y*mat[1][2] + z*mat[2][2] + w*mat[3][2];
  ret[3] = x*mat[0][3] + y*mat[1][3] + z*mat[2][3] + w*mat[3][3];

  return ret;
}
