#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <math.h>
#include <cassert>
#include "matrix3.h"

namespace tools
{

   ///////////////////////////////////////////////////
   /// euclidean tridimensional vector representation
   ///////////////////////////////////////////////////
   class Vector3
   {
     public:

      static const Vector3 ZERO;
      static const Vector3 X;
      static const Vector3 Y;
      static const Vector3 Z;

      Real x, y, z;

      Vector3() {}

     Vector3(const Real &a1, const Real &a2, const Real &a3): x(a1), y(a2), z(a3) {}
     Vector3(const Real vec[3]): x(vec[0]), y(vec[1]), z(vec[2]) {}
     Vector3(const Vector3 &v): x(v[0]), y(v[1]), z(v[2]) {}
      ~Vector3() {}

      inline Real operator [](const int &i) const
      {
	 assert(i >= 0 && i < 3);
	 
	 if (i == 0) return x;
	 if (i == 1) return y;
	 //if (i == 2) 
	 return z;
      }		
			
      inline Real &operator [](const int &i) 
      {
	 assert(i >= 0 && i < 3);
	 
	 if (i == 0) return *(&x);
	 if (i == 1) return *(&y);
	 //if (i == 2) 
	 return *(&z);
      }		
			
      inline Vector3 &operator =(const Vector3 &vec)
	 {
	    x = vec.x;
	    y = vec.y;
	    z = vec.z;
					
	    return *this;
	 }
			
      inline Vector3 operator +(const Vector3 &vec) const
      {
	 return Vector3(x + vec.x, y + vec.y, z + vec.z);
      }

      inline Vector3 operator -(const Vector3 &vec) const
      {
	 return Vector3(x - vec.x, y - vec.y, z - vec.z);
      }

      inline Vector3 operator *(const Real &scalar) const
      {
	 return Vector3(scalar*x, scalar*y, scalar*z);
      }

			
      inline Vector3 operator /(const Real &scalar) const
      {
	 return Vector3(x/scalar, y/scalar, z/scalar);
      }

      inline Vector3 operator +() const
      {
	 return *this;
      }

      inline Vector3 operator -() const
      {
	 return Vector3(-x, -y, -z);
      }

      inline Vector3 &operator +=(const Vector3 &vec)
      {
	 x += vec.x;
	 y += vec.y;
	 z += vec.z;

	 return *this;
      }

      inline Vector3 &operator -=(const Vector3 &vec)
      {
	 x -= vec.x;
	 y -= vec.y;
	 z -= vec.z;

	 return *this;
      }

      inline Vector3 &operator *=(const Real &scalar)
      {
	 x *= scalar;
	 y *= scalar;
	 z *= scalar;
					
	 return *this;
      }

      inline Vector3 &operator /=(const Real &scalar)
      {
	 x /= scalar;
	 y /= scalar;
	 z /= scalar;
					
	 return *this;
      }
      
      //dot product
      inline Real operator *(const Vector3 &vec) const
      {
	 return x*vec[0] + y*vec[1] + z*vec[2];
      }

      //cross product
      inline Vector3 operator ^(const Vector3 &vec) const
      {
	 return Vector3(y*vec[2] - z*vec[1],
			z*vec[0] - x*vec[2],
			x*vec[1] - y*vec[0]);
      }

      Vector3 operator *(const Matrix3 &mat) const;

     protected:

   };

   Real length(const Vector3 &vec);
   Vector3 normalize(const Vector3 &vec);

}

#endif //__VECTOR3_H__
