#include "algebra/matrixn.h"

using namespace tools;
using namespace std;

cMatrixN tools::conjugate(const cMatrixN &mat)
{
   cMatrixN ret(mat.rows(), mat.columns());

   for (unsigned i = 0; i < mat.rows(); i++)
   {
      for (unsigned j = 0; j < mat.columns(); j++)
	 ret[i][j] = conj(mat[i][j]);
   }
   return ret;
}

cMatrixN tools::adjoint(const cMatrixN &mat)
{
   cMatrixN ret(transpose(mat));

   ret = conjugate(ret);
	 
   return ret;
}
