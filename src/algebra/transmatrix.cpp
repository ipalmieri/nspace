#include "algebra/transmatrix.h"
#include "algebra/vector3.h"
#include "algebra/vector4.h"
#include "algebra/quaternion.h"

using namespace tools;

void transMatrix::setPosition(const Vector3 &pos)
{
   _matrix[0][3] = pos[0];
   _matrix[1][3] = pos[1];
   _matrix[2][3] = pos[2];
}

//fix this
void transMatrix::setRotation(const Radian &yaw, const Radian &pitch, const Radian &roll)
{
   transMatrix Rx, Ry, Rz;

   Rx = rotationMatrix(Vector3::X, roll);
   Ry = rotationMatrix(Vector3::Y, pitch);
   Rz = rotationMatrix(Vector3::Z, yaw);

   (*this) = Rx*Ry*Rz;

}

void transMatrix::setRotation(const Quaternion &q)
{
   Matrix4 mat = q.getMatrix();

   _matrix[0][0] = mat[0][0];
   _matrix[0][1] = mat[0][1];
   _matrix[0][2] = mat[0][2];

   _matrix[1][0] = mat[1][0];
   _matrix[1][1] = mat[1][1];
   _matrix[1][2] = mat[1][2];

   _matrix[2][0] = mat[2][0];
   _matrix[2][1] = mat[2][1];
   _matrix[2][2] = mat[2][2];
}

transMatrix transMatrix::rotationMatrix(const Vector3 &vec, const Radian &angle)
{
   Vector3 nvec = normalize(vec);

   Real cosA = cos(angle);
   Real sinA = sin(angle);
   
   Matrix4 mat;

   mat[0][0] = (1.0f-cosA)*nvec[0]*nvec[0] + cosA;
   mat[1][0] = (1.0f-cosA)*nvec[0]*nvec[1] + sinA*nvec[2];
   mat[2][0] = (1.0f-cosA)*nvec[0]*nvec[2] - sinA*nvec[1];
   mat[3][0] = 0.0f;

   mat[0][1] = (1.0f-cosA)*nvec[0]*nvec[1] - sinA*nvec[2];
   mat[1][1] = (1.0f-cosA)*nvec[1]*nvec[1] + cosA;
   mat[2][1] = (1.0f-cosA)*nvec[1]*nvec[2] + sinA*nvec[0];
   mat[3][1] = 0.0f;

   mat[0][2] = (1.0f-cosA)*nvec[0]*nvec[2] + sinA*nvec[1];
   mat[1][2] = (1.0f-cosA)*nvec[1]*nvec[2] - sinA*nvec[0];
   mat[2][2] = (1.0f-cosA)*nvec[2]*nvec[2] + cosA;
   mat[3][2] = 0.0f;

   mat[0][3] = 0.0f;
   mat[1][3] = 0.0f;
   mat[2][3] = 0.0f;
   mat[3][3] = 1.0f;

   return mat;	 
}

Real *transMatrix::getData() const
{
   return (Real *) _mat;
}
