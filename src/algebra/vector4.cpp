#include "algebra/vector4.h"

using namespace tools;

const Vector4 Vector4::ZERO(0, 0, 0, 0);

Real tools::length(const Vector4 &vec)
{
   return sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2] + vec[3]*vec[3]);
}

Vector4 tools::normalize(const Vector4 &vec)
{
   Real len = length(vec);

   return (len > 0.0)? vec/len : vec;
}
