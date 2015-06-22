#include "algebra/quaternion.h"
#include "math.h"
#include "algebra/vector3.h"
#include "algebra/matrix4.h"

using namespace tools;

Quaternion::Quaternion(const Real &a, const Real &b, const Real &c, const Real &d)
{
   x = a;
   y = b;
   z = c;
   w = d;
}
    
Quaternion::Quaternion(const Vector3 &vec, const Radian &angle) 
{
   float aangle = angle*0.5;

   float sina = sin(aangle);
   
   Vector3 nvec = tools::normalize(vec);
       
   x = nvec[0] * sina;
   y = nvec[1] * sina;
   z = nvec[2] * sina;
   w = cos(aangle);
}

void Quaternion::normalize()
{
   Real n = sqrt(x*x + y*y + z*z + w*w);

   if (n > 0.0)
   {
      x /= n;
      y /= n;
      z /= n;
      w /= n; 
   } 
}

Quaternion Quaternion::operator*(const Quaternion &q) const
{
   return Quaternion(w * q.x + x * q.w + y * q.z - z * q.y,
		     w * q.y + y * q.w + z * q.x - x * q.z,
		     w * q.z + z * q.w + x * q.y - y * q.x,
		     w * q.w - x * q.x - y * q.y - z * q.z);
}

//fix this
Matrix4 Quaternion::getMatrix() const
{
	float x2 = x*x;
	float y2 = y*y;
	float z2 = z*z;
	float w2 = w*w;
	float xy = x*y;
	float xz = x*z;
	float yz = y*z;
	float wx = w*x;
	float wy = w*y;
	float wz = w*z;
 
	return Matrix4(w2 + x2 - y2 - z2, 2.0f*(xy - wz), 2.0f*(xz + wy), 0.0f,
		       2.0f*(xy + wz), w2 - x2 + y2 - z2, 2.0f*(yz - wx), 0.0f,
		       2.0f*(xz - wy), 2.0f*(yz - wx), w2 - x2 - y2 + z2, 0.0f,
		       0.0f, 0.0f, 0.0f, 1.0f);
}

Quaternion Quaternion::inverse() const
{
   Quaternion q = this->conjugate();
   Real n = norm();
   n = n*n;
   
   if (n > 0.0)
   {
      q.x /= n;
      q.y /= n;
      q.z /= n;
      q.w /= n;
   }

   return q;
}

Vector3 Quaternion::rotate(const Vector3 &vec) const
{
   Quaternion p(vec[0], vec[1], vec[2], 0.0);

   p = (*this)*p*(this->inverse());
   
   return Vector3(p.x, p.y, p.z);
}
