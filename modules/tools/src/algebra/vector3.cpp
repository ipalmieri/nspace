#include "algebra/vector3.h"
#include "algebra/matrix3.h"

using namespace std;
using namespace tools;

const Vector3 Vector3::ZERO(0, 0, 0);
const Vector3 Vector3::X(1, 0, 0);
const Vector3 Vector3::Y(0, 1, 0);
const Vector3 Vector3::Z(0, 0, 1);

Real tools::length(const Vector3 &vec)
{
   return sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
}

Vector3 tools::normalize(const Vector3 &vec)
{
   Real len = length(vec);

   return (len > 0.0)? vec/len : vec;
}
			
      
Vector3 Vector3::operator *(const Matrix3 &mat) const
{
   Vector3 ret;
                                               
   ret[0] = x*mat[0][0] + y*mat[1][0] + z*mat[2][0];
   ret[1] = x*mat[0][1] + y*mat[1][1] + z*mat[2][1];
   ret[2] = x*mat[0][2] + y*mat[1][2] + z*mat[2][2];
        
   return ret;
}
      
