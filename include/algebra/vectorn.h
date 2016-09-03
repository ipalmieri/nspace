#ifndef __VECTORN_H__
#define __VECTORN_H__

#include <type_traits>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <cassert>
#include "adefs.h"

#define AL_MAX_VSIZE 104857600

namespace tools {
//////////////////////////////////////////////
/// vectorN is a multi length vector type
/// for hilbert space algebra
//////////////////////////////////////////////
template<typename T> class vectorN
{
 public:

  explicit vectorN(const unsigned& n, const T& value=0)
  {
    _vector = (T*) NULL;
    _size = 0;

    init(n);

    for (unsigned i = 0; i < _size; i++) {
      _vector[i] = value;
    }
  }

  vectorN(const vectorN<T>& vec)
  {
    _vector = (T*) NULL;
    _size = 0;

    init(vec.size());

    for (unsigned i = 0; i < _size; i++) {
      _vector[i] = vec[i];
    }
  }

  virtual ~vectorN()
  {
    if (_vector) {
      free(_vector);
    }
  }

  inline unsigned size() const
  {
    return _size;
  }

  inline T& operator[](const unsigned& index)
  {
    assert(index < _size);
    return _vector[index];
  }

  inline T operator[](const unsigned& index) const
  {
    assert(index < _size);
    return _vector[index];
  }

  vectorN<T>& operator=(const vectorN<T>& vec)
  {
    if (_size != vec.size()) {
      init(vec.size());
    }

    for (unsigned i = 0; i < _size; i++) {
      _vector[i] = vec[i];
    }

    return (*this);
  }

  vectorN<T> operator+(const vectorN<T>& vec) const
  {
    vectorN<T> sum(_size);

    assert(_size == vec.size());

    for (unsigned i = 0; i < _size; i++) {
      sum[i] = _vector[i] + vec[i];
    }

    return sum;
  }

  vectorN<T> operator-(const vectorN<T>& vec) const
  {
    vectorN<T> sum(_size);

    assert(_size == vec.size());

    for (unsigned i = 0; i < _size; i++) {
      sum[i] = _vector[i] - vec[i];
    }

    return sum;
  }

  //dot product
  T operator*(const vectorN<T>& vec) const
  {
    T ret = (T) 0;

    assert(_size == vec.size());

    for (unsigned i = 0; i < _size; i++) {
      ret += _vector[i]*vec[i];
    }

    return ret;
  }

  vectorN<T> operator*(const T& scalar) const
  {
    vectorN<T> prod(_size);

    for (unsigned i = 0; i < _size; i++) {
      prod[i] = _vector[i]*scalar;
    }

    return prod;
  }

  friend vectorN<T> operator*(const T& scalar, const vectorN<T>& vec)
  {
    return vec*scalar;
  }

  vectorN<T> operator/(const T& scalar) const
  {
    vectorN<T> prod(_size);

    for (unsigned i = 0; i < _size; i++) {
      prod[i] = _vector[i]/scalar;
    }

    return prod;
  }


  vectorN<T> operator+()
  {
    return *this;
  }

  vectorN<T> operator-()
  {
    vectorN<T> ret(_size);

    for (unsigned i = 0; i < ret.size(); i++) {
      ret[i] = - _vector[i];
    }

    return ret;
  }

  vectorN<T>& operator+=(const vectorN<T>& vec)
  {
    return *this + vec;
  }
  vectorN<T>& operator-=(const vectorN<T>& vec)
  {
    return *this - vec;
  }
  vectorN<T>& operator*=(const T& scalar)
  {
    return (*this)*scalar;
  }
  vectorN<T>& operator*=(const vectorN<T>& vec)
  {
    return (*this)*vec;
  }
  vectorN<T>& operator/=(const T& scalar)
  {
    return (*this)/scalar;
  }


 protected:

  void init(const unsigned& n)
  {
    unsigned nsize = (unsigned) std::min((unsigned)AL_MAX_VSIZE,
                                         std::max((unsigned)0, n));

    T* tvec = (T*) realloc(_vector, nsize*sizeof(T));

    if (tvec) {
      _vector = tvec;
      _size = nsize;
    }
  }


  T* _vector;
  unsigned _size; //N elements
};


//////////////////////////////////////////////////////////////
// Auxiliary functions
//////////////////////////////////////////////////////////////

template<typename T> inline T length(const vectorN<T>& vec)
{
  T ret = 0;

  for (unsigned i = 0; i < vec.size(); i++) {
    ret += vec[i]*std::conj(vec[i]);
  }

  return std::sqrt(ret);
}

template<typename T> inline T norm(const vectorN<T>& vec)
{
  return length(vec);
}

template<typename T> inline vectorN<T> conjugate(const vectorN<T>& vec)
{
  vectorN<T> ret(vec.size());

  for (unsigned i = 0; i < vec.size(); i++) {
    ret[i] = std::conj(vec[i]);
  }

  return ret;
}

template<typename T> inline vectorN<T> normalize(const vectorN<T>& vec)
{
  T len = length(vec);

  return (len > 0)? vec/len : vec;
}

typedef vectorN<Real> rvectorN;
typedef vectorN<Complex> cvectorN;

}

#endif
