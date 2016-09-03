#include "algebra/matrixn.h"

using namespace tools;
using namespace std;

cmatrixN tools::conjugate(const cmatrixN& mat)
{
  cmatrixN ret(mat.rows(), mat.columns());

  for (unsigned i = 0; i < mat.rows(); i++) {
    for (unsigned j = 0; j < mat.columns(); j++) {
      ret[i][j] = std::conj(mat[i][j]);
    }
  }
  return ret;
}

cmatrixN tools::adjoint(const cmatrixN& mat)
{
  cmatrixN ret(transpose(mat));

  ret = conjugate(ret);

  return ret;
}
