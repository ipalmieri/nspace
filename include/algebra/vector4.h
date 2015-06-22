#ifndef __VECTOR4_H__
#define __VECTOR4_H__

#include <math.h>
#include <cassert>
#include "adefs.h"
#include "vector3.h"

namespace tools
{
   class Matrix4;

   //////////////////////////////////////////////////////
   /// euclidean fourth-dimensional vector representation
   //////////////////////////////////////////////////////
   class Vector4
   {
     public:
			
      static const Vector4 ZERO;

      Real x, y, z, w;

      Vector4() {}

     Vector4(const Real &a1, const Real &a2, const Real &a3, const Real &a4): x(a1), y(a2), z(a3), w(a4) {}
     Vector4(const Real vec[4]): x(vec[0]), y(vec[1]), z(vec[2]), w(vec[3]) {}
     Vector4(const Vector3 &v, const Real &nw=1.0f): x(v[0]), y(v[1]), z(v[2]), w(nw) {}
      ~Vector4() {}

      inline Real operator [](const int &i) const
      {
	 assert(i >= 0 && i < 4);
				 
	 if (i == 0) return x;
	 if (i == 1) return y;
	 if (i == 2) return z;
	 /*if (i == 3)*/ return w;

      }		
			
      inline Real &operator [](const int &i) 
      {
	 assert(i >= 0 && i < 4);
				 
	 if (i == 0) return *(&x);
	 if (i == 1) return *(&y);
	 if (i == 2) return *(&z);
	 /*if (i == 3)*/ return *(&w);

      }		
			
      inline Vector4 &operator =(const Vector4 &vec)
	 {
	    x = vec.x;
	    y = vec.y;
	    z = vec.z;
	    w = vec.w;
					
	    return *this;
	 }
			
      inline Vector4 operator +(const Vector4 &vec) const
      {
	 return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
      }

      inline Vector4 operator -(const Vector4 &vec) const
      {
	 return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
      }

      inline Vector4 operator *(const Real &scalar) const
      {
	 return Vector4(scalar*x, scalar*y, scalar*z, scalar*w);
      }

			
      inline Vector4 operator /(const Real &scalar) const
      {
	 return Vector4(x/scalar, y/scalar, z/scalar, w/scalar);
      }

      inline Vector4 operator +() const
      {
	 return *this;
      }

      inline Vector4 operator -() const
      {
	 return Vector4(-x, -y, -z, -w);
      }

      inline Vector4 &operator +=(const Vector4 &vec)
      {
	 x += vec.x;
	 y += vec.y;
	 z += vec.z;
	 w += vec.w;
          
	 return *this;
      }

      inline Vector4 &operator -=(const Vector4 &vec)
      {
	 x -= vec.x;
	 y -= vec.y;
	 z -= vec.z;
	 w -= vec.w;
          
	 return *this;
      }

      inline Vector4 &operator *=(const Real &scalar)
      {
	 x *= scalar;
	 y *= scalar;
	 z *= scalar;
	 w *= scalar;
					
	 return *this;
      }

      inline Vector4 &operator /=(const Real &scalar)
      {
	 x /= scalar;
	 y /= scalar;
	 z /= scalar;
	 w /= scalar;
					
	 return *this;
      }
			
      //dot product
      inline Real operator *(const Vector4 &vec) const
      {
	 return x*vec.x + y*vec.y + z*vec.z + w*vec.w;
      }			

      //operator * por matrix4;

     protected:
		
   };

   Real length(const Vector4 &vec);
   Vector4 normalize(const Vector4 &vec);
  
} //__VECTOR4_H__

#endif
