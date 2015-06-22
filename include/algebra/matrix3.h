#ifndef __MATRIX3_H__
#define __MATRIX3_H__

#include "adefs.h"


namespace tools
{
   class Vector3;

   //////////////////////////////////////////////
   /// 3x3 matrix representation
   //////////////////////////////////////////////
   class Matrix3
   {
     public:

      static const Matrix3 ZERO;
      static const Matrix3 IDENTITY;

      Matrix3(Real m[3][3])
      {
	 for (unsigned i = 0; i < 3; i++)
	 {
	    for (unsigned j = 0; j < 3; j++)
	       _matrix[i][j] = m[i][j];
	 }
      }

      Matrix3(const Matrix3 &mat=ZERO)
      {
	 for (unsigned i = 0; i < 3; i++)
	 {
	    for (unsigned j = 0; j < 3; j++)
	       _matrix[i][j] = mat._matrix[i][j];
	 }				
      }

      Matrix3(Real a00, Real a01, Real a02,
	      Real a10, Real a11, Real a12,
	      Real a20, Real a21, Real a22)
      {
	 _matrix[0][0] = a00;
	 _matrix[0][1] = a01;
	 _matrix[0][2] = a02;
	 _matrix[1][0] = a10;
	 _matrix[1][1] = a11;
	 _matrix[1][2] = a12;
	 _matrix[2][0] = a20;
	 _matrix[2][1] = a21;
	 _matrix[2][2] = a22;

      }

      ~Matrix3() {};

      inline Real *operator[](const unsigned &index) const
      {
	 return (Real *)_matrix[index];
      } 

      Matrix3 operator+(const Matrix3 &mat) const;
      Matrix3 operator-(const Matrix3 &mat) const;
      Matrix3 operator*(const Matrix3 &mat) const;
      Matrix3 operator*(const Real &scalar) const;
      friend Matrix3 operator*(const Real &scalar, const Matrix3 &mat);
      Vector3 operator*(const Vector3 &vec) const;
      friend Vector3 operator*(const Vector3 &vec, const Matrix3 &mat);
		
      Matrix3 transpose() const;
      Matrix3 inverse() const;
      Real determinant() const;

     protected:

      union {
	 Real _matrix[3][3];
	 Real _mat[9];
      };

   };
}

#endif //__MATRIX3_H__
