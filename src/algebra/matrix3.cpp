#include "algebra/matrix3.h"
#include "algebra/vector3.h"

using namespace std;
using namespace tools;

const Matrix3 Matrix3::ZERO(0, 0, 0,
			    0, 0, 0,
			    0, 0, 0);

const Matrix3 Matrix3::IDENTITY(1, 0, 0,
				0, 1, 0,
				0, 0, 1);

Matrix3 Matrix3::operator+(const Matrix3 &mat) const
{
   Matrix3 sum;

   for (unsigned i = 0; i < 3; i++)
   {
      for (unsigned j = 0; j < 3; j++)
	 sum[i][j] = _matrix[i][j] + mat[i][j];
   }
				
   return sum;
}	

Matrix3 Matrix3::operator-(const Matrix3 &mat) const
{
   Matrix3 diff;

   for (unsigned i = 0; i < 3; i++)
   {
      for (unsigned j = 0; j < 3; j++)
	 diff[i][j] = _matrix[i][j] - mat[i][j];
   }

   return diff;
}

Matrix3 Matrix3::operator*(const Matrix3 &mat) const
{
   Matrix3 prod;

   for (unsigned i = 0; i < 3; i++)
   {
      for (unsigned j = 0; j < 3; j++)
      {
	 prod[i][j] = 0.0;

	 for (unsigned k = 0; k < 3; k++)
	    prod[i][j] += _matrix[i][k]*mat[k][j];

      }
   }

   return prod;
}

Matrix3 Matrix3::operator*(const Real &scalar) const
{
   Matrix3 prod;

   for (unsigned i = 0; i < 3; i++)
   {
      for (unsigned j = 0; j < 3; j++)
	 prod[i][j] = _matrix[i][j]*scalar;
   }
   return prod;	
}

Matrix3 tools::operator*(const Real &scalar, const Matrix3 &mat)
{
   Matrix3 prod;

   for (unsigned i = 0; i < 3; i++)
   {
      for (unsigned j = 0; j < 3; j++)
	 prod[i][j] = mat[i][j]*scalar;
   }
   return prod;
}

Vector3 Matrix3::operator*(const Vector3 &vec) const
{
   Vector3 ret;

   ret[0] = _matrix[0][0]*vec[0] + _matrix[0][1]*vec[1] + _matrix[0][2]*vec[2];
   ret[1] = _matrix[1][0]*vec[0] + _matrix[1][1]*vec[1] + _matrix[1][2]*vec[2];
   ret[2] = _matrix[2][0]*vec[0] + _matrix[2][1]*vec[1] + _matrix[2][2]*vec[2]; 

   return ret;
}

Vector3 tools::operator*(const Vector3 &vec, const Matrix3 &mat)
{
   Vector3 ret;

   ret[0] = vec[0]*mat[0][0] + vec[1]*mat[1][0] + vec[2]*mat[2][0]; 
   ret[1] = vec[0]*mat[0][1] + vec[1]*mat[1][1] + vec[2]*mat[2][1];
   ret[2] = vec[0]*mat[0][2] + vec[1]*mat[1][2] + vec[2]*mat[2][2];
                                               
   return ret;
}

Matrix3 Matrix3::transpose() const
{
   Matrix3 mtrans;

   for (unsigned i = 0; i < 3; i++)
   {
      for (unsigned j = 0; j < 3; j++)
	 mtrans[i][j] = _matrix[j][i];
   }

   return mtrans;
}

Matrix3 Matrix3::inverse() const
{
   Matrix3 inverse;

   inverse[0][0] = _matrix[1][1]*_matrix[2][2] - _matrix[1][2]*_matrix[2][1];
   inverse[0][1] = _matrix[0][2]*_matrix[2][1] - _matrix[0][1]*_matrix[2][2];
   inverse[0][2] = _matrix[0][1]*_matrix[1][2] - _matrix[0][2]*_matrix[1][1];
   inverse[1][0] = _matrix[1][2]*_matrix[2][0] - _matrix[1][0]*_matrix[2][2];
   inverse[1][1] = _matrix[0][0]*_matrix[2][2] - _matrix[0][2]*_matrix[2][0];
   inverse[1][2] = _matrix[0][2]*_matrix[1][0] - _matrix[0][0]*_matrix[1][2];
   inverse[2][0] = _matrix[1][0]*_matrix[2][1] - _matrix[1][1]*_matrix[2][0];
   inverse[2][1] = _matrix[0][1]*_matrix[2][0] - _matrix[0][0]*_matrix[2][1];
   inverse[2][2] = _matrix[0][0]*_matrix[1][1] - _matrix[0][1]*_matrix[1][0];

   Real det = _matrix[0][0]*inverse[0][0] + _matrix[0][1]*inverse[1][0] + _matrix[0][2]*inverse[2][0];

   if (realAbs(det) == 0.0)
   {
      return *this;
   }

   Real idet = 1.0/det;

   for (unsigned i = 0; i < 3; i++)
   {
      for (unsigned j = 0; j < 3; j++)
      {	
	 inverse[i][j] *= idet;  
      }
   }

   return inverse;
	
}

Real Matrix3::determinant() const
{
   Real cofactor00 = _matrix[1][1]*_matrix[2][2] - _matrix[1][2]*_matrix[2][1];
   Real cofactor10 = _matrix[1][2]*_matrix[2][0] - _matrix[1][0]*_matrix[2][2];
   Real cofactor20 = _matrix[1][0]*_matrix[2][1] - _matrix[1][1]*_matrix[2][0];

   Real det = _matrix[0][0]*cofactor00 + _matrix[0][1]*cofactor10 + _matrix[0][2]*cofactor20;

   return det;

}
