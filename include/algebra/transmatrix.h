#ifndef __TRANSMATRIX_H__
#define __TRANSMATRIX_H__

#include "adefs.h"
#include "matrix4.h"

namespace tools {
class Quaternion;
class vector3;

/////////////////////////////////////////////
/// rotation, scaling and translation matrix
/////////////////////////////////////////////
class transMatrix: public matrix4
{
 public:
  transMatrix(const matrix4& mat=IDENTITY):matrix4(mat) {}
  ~transMatrix() {}

  void setPosition(const vector3& pos);
  void setRotation(const Radian& yaw, const Radian& pitch, const Radian& roll);
  void setRotation(const Quaternion& q);

  static transMatrix rotationMatrix(const vector3& vec, const Radian& angle);

  Real* getData() const;

};
}

#endif //__TRANSMATRIX_H__
