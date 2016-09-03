#ifndef __MATRIXN_H__
#define __MATRIXN_H__

#define AL_MAX_MWIDTH 1048576
#define AL_MAX_MHEIGHT 1048576

#include <type_traits>
#include <stdlib.h>
#include <algorithm>
#include <cassert>
#include "adefs.h"
#include "vectorn.h"


namespace tools {

//////////////////////////////////////////////
/// matrixN is a multi length matrix type
/// for hilbert space algebra
//////////////////////////////////////////////
template<typename T> class matrixN
{
 public:

  explicit matrixN(const unsigned& n, const unsigned& m, const T& value = (T)0)
  {
    init(n, m);

    for (unsigned i = 0; i < _sizeN; i++) {
      for (unsigned j = 0; j < _sizeM; j++) {
        _matrix[i][j] = value;
      }
    }
  }

  matrixN(const matrixN<T>& mat)
  {
    init(mat.rows(), mat.columns());

    for (unsigned i = 0; i < _sizeN; i++) {
      for (unsigned j = 0; j < _sizeM; j++) {
        _matrix[i][j] = mat._matrix[i][j];
      }
    }
  }

  matrixN(const vectorN<T>& vec)
  {
    init(vec.size(), 1);

    for (unsigned i = 0; i < _sizeN; i++) {
      _matrix[i][0] = vec[i];
    }
  }

  virtual ~matrixN()
  {
    for (unsigned i = 0; i < _sizeN; i++) {
      free(_matrix[i]);
    }

    free(_matrix);
  }

  inline T* operator[](const unsigned& index) const
  {
    assert(index < _sizeN);

    return (T*) _matrix[index];
  }


  matrixN<T>& operator=(const matrixN<T>& mat)
  {
    assert(_sizeN == mat.rows() && _sizeM == mat.columns());

    for (unsigned i = 0; i < _sizeN; i++) {
      for (unsigned j = 0; j < _sizeM; j++) {
        _matrix[i][j] = mat[i][j];
      }
    }

    return (*this);
  }

  matrixN<T> operator+(const matrixN<T>& mat) const
  {
    matrixN<T> sum(_sizeN, _sizeM);

    assert(_sizeN == mat.rows() && _sizeM == mat.columns());

    for (unsigned i = 0; i < _sizeN; i++) {
      for (unsigned j = 0; j < _sizeM; j++) {
        sum[i][j] = _matrix[i][j] + mat[i][j];
      }
    }
    return sum;
  }


  matrixN<T> operator-(const matrixN<T>& mat) const
  {
    matrixN<T> sum(_sizeN, _sizeM);

    //fix this: must check for same size
    assert(_sizeN == mat.rows() && _sizeM == mat.columns());

    for (unsigned i = 0; i < _sizeN; i++) {
      for (unsigned j = 0; j < _sizeM; j++) {
        sum[i][j] = _matrix[i][j] - mat[i][j];
      }
    }
    return sum;
  }

  //fix this: naive
  matrixN<T> operator*(const matrixN<T>& mat) const
  {
    matrixN<T> ret(_sizeN, mat.columns());

    assert(_sizeM == mat.rows());

    for (unsigned i = 0; i < ret.rows(); i++) {
      for (unsigned j = 0; j < ret.columns(); j++) {
        ret[i][j] = (T) 0;
        for (unsigned k = 0; k < _sizeM; k++) {
          ret[i][j] += _matrix[i][k] * mat[k][j];
        }
      }
    }
    return ret;
  }

  vectorN<T> operator*(const vectorN<T>& vec) const
  {
    vectorN<T> ret(_sizeN);

    assert(_sizeM == vec.size());

    for (unsigned i = 0; i < _sizeN; i++) {
      ret[i] = (T) 0;
      for (unsigned j = 0; j < _sizeM; j++) {
        ret[i] += _matrix[i][j] * vec[j];
      }
    }
    return ret;
  }

  matrixN<T> operator*(const T& scalar) const
  {
    matrixN<T> prod(_sizeN, _sizeM);

    for (unsigned i = 0; i < prod.rows(); i++) {
      for (unsigned j = 0; j < prod.columns(); j++) {
        prod[i][j] = _matrix[i][j] * scalar;
      }
    }
    return prod;
  }

  friend matrixN<T> operator*(const T& scalar, const matrixN<T>& mat)
  {
    return mat * scalar;
  }

  matrixN<T> operator /(const T& scalar) const
  {
    matrixN<T> prod(_sizeN, _sizeM);

    for (unsigned i = 0; i < _sizeN; i++) {
      for (unsigned j = 0; j < _sizeM; j++) {
        prod[i][j] = _matrix[i][j]/scalar;
      }
    }
    return prod;
  }

  matrixN<T> operator+()
  {
    return *this;
  }

  matrixN<T> operator-()
  {
    matrixN<T> ret(_sizeN, _sizeM);

    for (unsigned i = 0; i < ret.rows(); i++) {
      for (unsigned j = 0; j < ret.columns(); j++) {
        ret[i][j] = - _matrix[i][j];
      }
    }

    return ret;
  }

  matrixN<T>& operator+=(const matrixN<T>& mat)
  {
    return *this + mat;
  }
  matrixN<T>& operator-=(const matrixN<T>& mat)
  {
    return *this - mat;
  }
  matrixN<T>& operator*=(const T& scalar)
  {
    return (*this)*scalar;
  }
  matrixN<T>& operator*=(const matrixN<T>& mat)
  {
    return (*this)*mat;
  }
  matrixN<T>& operator/=(const T& scalar)
  {
    return (*this)/scalar;
  }

  inline unsigned rows() const
  {
    return _sizeN;
  }
  inline unsigned columns() const
  {
    return _sizeM;
  }

 protected:

  void init(const unsigned& n, const unsigned& m)
  {
    _sizeN = std::min((unsigned)AL_MAX_MHEIGHT, std::max((unsigned)0, n));
    _sizeM = std::min((unsigned)AL_MAX_MWIDTH, std::max((unsigned)0, m));

    _matrix = (T**) malloc(_sizeN*sizeof(T*));

    for (unsigned i = 0; i < _sizeN; i++) {
      _matrix[i] = (T*) malloc(_sizeM*sizeof(T));
    }
  }

  T** _matrix;
  unsigned _sizeN, _sizeM; //N lines x M cols

};


template<typename T> matrixN<T> identity(const unsigned& n)
{
  matrixN<T> ret(n, n);

  for (unsigned i = 0; i < ret.rows(); i++) {
    ret[i][i] = (T) 1;
  }

  return ret;
}

//bool isInvertible();
//virtual matrixN<T> inverse() const;
//virtual T determinant();

template<typename T> matrixN<T> transpose(const matrixN<T>& mat)
{
  matrixN<T> ret(mat.columns(), mat.rows());

  for (unsigned i = 0; i < ret.rows(); i++) {
    for (unsigned j = 0; j < ret.columns(); j++) {
      ret[i][j] = mat[j][i];
    }
  }
  return ret;
}

template<typename T> matrixN<T> diagonal(const vectorN<T>& vec)
{
  matrixN<T> ret(vec.size(), vec.size());

  for (unsigned i = 0; i < ret.rows(); i++) {
    ret[i][i] = vec[i];
  }

  return ret;
}

template<typename T> vectorN<T> diagonal(const matrixN<T>& mat)
{
  //fix this: must check for matching sizes
  assert(mat.rows() == mat.columns());

  vectorN<T> ret(mat.rows());

  for (unsigned i = 0; i < ret.size(); i++) {
    ret[i] = mat[i][i];
  }

  return ret;
}

template<typename T> T trace(const matrixN<T>& mat)
{
  T ret = (T) 0;

  //fix this: must check for matching sizes
  assert(mat.rows() == mat.columns());

  for (unsigned i = 0; i < mat.rows(); i++) {
    ret += mat[i][i];
  }

  return ret;
}

template<typename T> matrixN<T> conjugate(const matrixN<T>& mat)
{
  matrixN<T> ret(mat.rows(), mat.columns());

  for (unsigned i = 0; i < mat.rows(); i++) {
    for (unsigned j = 0; j < mat.columns(); j++) {
      ret[i][j] = std::conj(mat[i][j]);
    }
  }

  return ret;
}

template<typename T> matrixN<T> adjoint(const matrixN<T>& mat)
{
  matrixN<T> ret(transpose(mat));

  ret = conjugate(ret);

  return ret;
}

typedef matrixN<Real> rmatrixN;
typedef matrixN<Complex> cmatrixN;

}

#endif
