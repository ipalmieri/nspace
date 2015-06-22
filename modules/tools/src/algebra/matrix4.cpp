#include "algebra/matrix4.h"
#include "algebra/vector4.h"

using namespace tools;

const Matrix4 Matrix4::ZERO(0, 0, 0, 0,
			    0, 0, 0, 0,
			    0, 0, 0, 0,
			    0, 0, 0, 0);

const Matrix4 Matrix4::IDENTITY(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);


Matrix4 Matrix4::operator+(const Matrix4 &mat) const
{
   Matrix4 sum;

   for (unsigned i = 0; i < 4; i++)
   {
      for (unsigned j = 0; j < 4; j++)
	 sum[i][j] = _matrix[i][j] + mat[i][j];
   }
				
   return sum;
}	

Matrix4 Matrix4::operator-(const Matrix4 &mat) const
{
   Matrix4 diff;

   for (unsigned i = 0; i < 4; i++)
   {
      for (unsigned j = 0; j < 4; j++)
	 diff[i][j] = _matrix[i][j] - mat[i][j];
   }
   return diff;
}

Matrix4 Matrix4::operator*(const Matrix4 &mat) const
{
   Matrix4 prod;

   for (unsigned i = 0; i < 4; i++)
   {
      for (unsigned j = 0; j < 4; j++)
      {
	 prod[i][j] = 0.0;

	 for (unsigned k = 0; k < 4; k++)
	    prod[i][j] += _matrix[i][k]*mat[k][j];

      }
   }

   return prod;
}

Matrix4 Matrix4::operator*(const Real &scalar) const
{
   Matrix4 prod;

   for (unsigned i = 0; i < 4; i++)
   {
      for (unsigned j = 0; j < 4; j++)
	 prod[i][j] = _matrix[i][j]*scalar;
   }
   return prod;	
}

Matrix4 tools::operator*(const Real &scalar, const Matrix4 &mat)
{
   return mat*scalar;
}

Vector4 Matrix4::operator*(const Vector4 &vec) const
{
   Vector4 ret;

   ret[0] = _matrix[0][0]*vec[0] + _matrix[0][1]*vec[1] + _matrix[0][2]*vec[2] + _matrix[0][3]*vec[3];
   ret[1] = _matrix[1][0]*vec[0] + _matrix[1][1]*vec[1] + _matrix[1][2]*vec[2] + _matrix[1][3]*vec[3];
   ret[2] = _matrix[2][0]*vec[0] + _matrix[2][1]*vec[1] + _matrix[2][2]*vec[2] + _matrix[2][3]*vec[3];
   ret[3] = _matrix[3][0]*vec[0] + _matrix[3][1]*vec[1] + _matrix[3][2]*vec[2] + _matrix[3][3]*vec[3];

   return ret;
}

Vector4 tools::operator*(const Vector4 &vec, const Matrix4 &mat)
{
   Vector4 ret;
  
   ret[0] = vec[0]*mat[0][0] + vec[1]*mat[1][0] + vec[2]*mat[2][0] + vec[3]*mat[3][0]; 
   ret[1] = vec[0]*mat[0][1] + vec[1]*mat[1][1] + vec[2]*mat[2][1] + vec[3]*mat[3][1];
   ret[2] = vec[0]*mat[0][2] + vec[1]*mat[1][2] + vec[2]*mat[2][2] + vec[3]*mat[3][2];
   ret[3] = vec[0]*mat[0][3] + vec[1]*mat[1][3] + vec[2]*mat[2][3] + vec[3]*mat[3][3];
       
   return ret;
}

Matrix4 Matrix4::transpose() const
{
   Matrix4 mtrans;

   for (unsigned i = 0; i < 4; i++)
   {
      for (unsigned j = 0; j < 4; j++)
	 mtrans[i][j] = _matrix[j][i];
   }

   return mtrans;
}

Matrix4 Matrix4::inverse() const
{
   Real v0 = _matrix[2][0]*_matrix[3][1] - _matrix[2][1]*_matrix[3][0];
   Real v1 = _matrix[2][0]*_matrix[3][2] - _matrix[2][2]*_matrix[3][0];
   Real v2 = _matrix[2][0]*_matrix[3][3] - _matrix[2][3]*_matrix[3][0];
   Real v3 = _matrix[2][1]*_matrix[3][2] - _matrix[2][2]*_matrix[3][1];
   Real v4 = _matrix[2][1]*_matrix[3][3] - _matrix[2][3]*_matrix[3][1];
   Real v5 = _matrix[2][2]*_matrix[3][3] - _matrix[2][3]*_matrix[3][2];

   Real t00 = + (v5*_matrix[1][1] - v4*_matrix[1][2] + v3*_matrix[1][3]);
   Real t10 = - (v5*_matrix[1][0] - v2*_matrix[1][2] + v1*_matrix[1][3]);
   Real t20 = + (v4*_matrix[1][0] - v2*_matrix[1][1] + v0*_matrix[1][3]);
   Real t30 = - (v3*_matrix[1][0] - v1*_matrix[1][1] + v0*_matrix[1][2]);
	
   Real idet = 1 / (t00*_matrix[0][0] + t10*_matrix[0][1] + t20*_matrix[0][2] + t30*_matrix[0][3]);

   Real d00 = t00*idet;
   Real d10 = t10*idet;
   Real d20 = t20*idet;	
   Real d30 = t30*idet;

   Real d01 = - (v5*_matrix[0][1] - v4*_matrix[0][2] + v3*_matrix[0][3])*idet;
   Real d11 = + (v5*_matrix[0][0] - v2*_matrix[0][2] + v1*_matrix[0][3])*idet;
   Real d21 = - (v4*_matrix[0][0] - v2*_matrix[0][1] + v0*_matrix[0][3])*idet;
   Real d31 = + (v3*_matrix[0][0] - v1*_matrix[0][1] + v0*_matrix[0][2])*idet;
	
   v0 = _matrix[1][0]*_matrix[3][1] - _matrix[1][1]*_matrix[3][0];
   v1 = _matrix[1][0]*_matrix[3][2] - _matrix[1][2]*_matrix[3][0];
   v2 = _matrix[1][0]*_matrix[3][3] - _matrix[1][3]*_matrix[3][0];
   v3 = _matrix[1][1]*_matrix[3][2] - _matrix[1][2]*_matrix[3][1];
   v4 = _matrix[1][1]*_matrix[3][3] - _matrix[1][3]*_matrix[3][1];
   v5 = _matrix[1][2]*_matrix[3][3] - _matrix[1][3]*_matrix[3][2];
		
   Real d02 = + (v5*_matrix[0][1] - v4*_matrix[0][2] + v3*_matrix[0][3])*idet;
   Real d12 = - (v5*_matrix[0][0] - v2*_matrix[0][2] + v1*_matrix[0][3])*idet;
   Real d22 = + (v4*_matrix[0][0] - v2*_matrix[0][1] + v0*_matrix[0][3])*idet;
   Real d32 = - (v3*_matrix[0][0] - v1*_matrix[0][1] + v0*_matrix[0][2])*idet;
	
   v0 = _matrix[2][1]*_matrix[1][0] - _matrix[2][0]*_matrix[1][1];
   v1 = _matrix[2][2]*_matrix[1][0] - _matrix[2][0]*_matrix[1][2];
   v2 = _matrix[2][3]*_matrix[1][0] - _matrix[2][0]*_matrix[1][3];
   v3 = _matrix[2][2]*_matrix[1][1] - _matrix[2][1]*_matrix[1][2];
   v4 = _matrix[2][3]*_matrix[1][1] - _matrix[2][1]*_matrix[1][3];
   v5 = _matrix[2][3]*_matrix[1][2] - _matrix[2][2]*_matrix[1][3];
		
   Real d03 = - (v5*_matrix[0][1] - v4*_matrix[0][2] + v3*_matrix[0][3])*idet;
   Real d13 = + (v5*_matrix[0][0] - v2*_matrix[0][2] + v1*_matrix[0][3])*idet;
   Real d23 = - (v4*_matrix[0][0] - v2*_matrix[0][1] + v0*_matrix[0][3])*idet;
   Real d33 = + (v3*_matrix[0][0] - v1*_matrix[0][1] + v0*_matrix[0][2])*idet;
	
	
   return Matrix4(d00, d01, d02, d03,
		  d10, d11, d12, d13,
		  d20, d21, d22, d23,
		  d30, d31, d32, d33);

	
}

Real Matrix4::determinant() const
{
   Real det;

   Real v0 = _matrix[2][0]*_matrix[3][1] - _matrix[2][1]*_matrix[3][0];
   Real v1 = _matrix[2][0]*_matrix[3][2] - _matrix[2][2]*_matrix[3][0];
   Real v2 = _matrix[2][0]*_matrix[3][3] - _matrix[2][3]*_matrix[3][0];
   Real v3 = _matrix[2][1]*_matrix[3][2] - _matrix[2][2]*_matrix[3][1];
   Real v4 = _matrix[2][1]*_matrix[3][3] - _matrix[2][3]*_matrix[3][1];
   Real v5 = _matrix[2][2]*_matrix[3][3] - _matrix[2][3]*_matrix[3][2];

   Real t00 = + (v5*_matrix[1][1] - v4*_matrix[1][2] + v3*_matrix[1][3]);
   Real t10 = - (v5*_matrix[1][0] - v2*_matrix[1][2] + v1*_matrix[1][3]);
   Real t20 = + (v4*_matrix[1][0] - v2*_matrix[1][1] + v0*_matrix[1][3]);
   Real t30 = - (v3*_matrix[1][0] - v1*_matrix[1][1] + v0*_matrix[1][1]);
	

   det = t00*_matrix[0][0];
   det += t10*_matrix[0][1];
   det += t20*_matrix[0][2];
   det += t30*_matrix[0][3];

   return det;

}
