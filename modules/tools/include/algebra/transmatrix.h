#ifndef __TRANSMATRIX_H__
#define __TRANSMATRIX_H__

#include "adefs.h"
#include "matrix4.h"

namespace tools
{
   class Quaternion;
   class Vector3;

   /////////////////////////////////////////////
   /// rotation, scaling and translation matrix
   /////////////////////////////////////////////
   class transMatrix: public Matrix4
   {
     public:
     transMatrix(const Matrix4 &mat=IDENTITY):Matrix4(mat) {}
      ~transMatrix() {}

      void setPosition(const Vector3 &pos);
      void setRotation(const Radian &yaw, const Radian &pitch, const Radian &roll);
      void setRotation(const Quaternion &q);
      
      static transMatrix rotationMatrix(const Vector3 &vec, const Radian &angle);

      Real *getData() const;
      
   };
}

#endif //__TRANSMATRIX_H__
