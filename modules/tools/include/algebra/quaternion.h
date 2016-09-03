#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include <cmath>
#include "adefs.h"

namespace tools {
class vector3;
class matrix4;

/////////////////////////////////////////////////
/// Quaternion class
/////////////////////////////////////////////////
class Quaternion
{
 public:

  Quaternion() {};
  ~Quaternion() {};

  Quaternion(const Real& a, const Real& b, const Real& c, const Real& d);
  Quaternion(const vector3& vec, const Radian& angle);

  inline Quaternion conjugate() const
  {
    return Quaternion(-x, -y, -z, w);
  }
  inline Real norm() const
  {
    return sqrt(x*x + y*y + z*z + w*w);
  }
  inline Quaternion inverse() const;
  void normalize();
  vector3 rotate(const vector3& vec) const;
  matrix4 getMatrix() const;

  Quaternion operator*(const Quaternion& rq) const;

  Real x;
  Real y;
  Real z;
  Real w;

 protected:

};
}

#endif //__QUATERNION_H__
