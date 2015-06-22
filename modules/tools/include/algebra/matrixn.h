#ifndef __MATRIXN_H__
#define __MATRIXN_H__

#define AL_MAX_MWIDTH 1048576
#define AL_MAX_MHEIGHT 1048576

#include <stdlib.h>
#include <algorithm>
#include <cassert>
#include "adefs.h"
#include "vectorn.h"


namespace tools
{

   //////////////////////////////////////////////
   /// MatrixN is a multi length matrix type
   /// for hilbert space algebra
   //////////////////////////////////////////////
   template<typename T> class MatrixN
   {
     public:
            
      explicit MatrixN(const unsigned &n, const unsigned &m, const T &value = (T)0)
      {
	 init(n, m);
	 
	 for (unsigned i = 0; i < _sizeN; i++)
	 {
	    for (unsigned j = 0; j < _sizeM; j++)
	       _matrix[i][j] = value;
	 }   
      }
      
      MatrixN(const MatrixN<T> &mat)
      {
	 init(mat.rows(), mat.columns());
	 
	 for (unsigned i = 0; i < _sizeN; i++)
	 {
	    for (unsigned j = 0; j < _sizeM; j++)
	       _matrix[i][j] = mat._matrix[i][j];
	 }   
      }

      MatrixN(const VectorN<T> &vec)
      {
	 init(vec.size(), 1);
	 
	 for (unsigned i = 0; i < _sizeN; i++)
	     _matrix[i][0] = vec[i];
      }
      
      virtual ~MatrixN()
      {
	 for (unsigned i = 0; i < _sizeN; i++)
	    free(_matrix[i]);

	 free(_matrix);
      }

      inline T *operator[](const unsigned &index) const
      {
	 assert(index < _sizeN);

	 return (T *) _matrix[index];
      }


      MatrixN<T> &operator=(const MatrixN<T> &mat) 
      {
	 assert(_sizeN == mat.rows() && _sizeM == mat.columns());

	 for (unsigned i = 0; i < _sizeN; i++)
	 {
	    for (unsigned j = 0; j < _sizeM; j++)
	       _matrix[i][j] = mat[i][j];
	 }   

	 return (*this);
      }
      
      MatrixN<T> operator+(const MatrixN<T> &mat) const
      {
	 MatrixN<T> sum(_sizeN, _sizeM);

	 assert(_sizeN == mat.rows() && _sizeM == mat.columns());

	 for (unsigned i = 0; i < _sizeN; i++)
	 {
	    for (unsigned j = 0; j < _sizeM; j++)
	       sum[i][j] = _matrix[i][j] + mat[i][j];
	 }
	 return sum;
      }


      MatrixN<T> operator-(const MatrixN<T> &mat) const
      {
	 MatrixN<T> sum(_sizeN, _sizeM);

	 //fix this: must check for same size
	 assert(_sizeN == mat.rows() && _sizeM == mat.columns());

	 for (unsigned i = 0; i < _sizeN; i++)
	 {
	    for (unsigned j = 0; j < _sizeM; j++)
	       sum[i][j] = _matrix[i][j] - mat[i][j];
	 }
	 return sum;
      }

      //fix this: naive
      MatrixN<T> operator*(const MatrixN<T> &mat) const
      {
	 MatrixN<T> ret(_sizeN, mat.columns());

	 assert(_sizeM == mat.rows());

	 for (unsigned i = 0; i < ret.rows(); i++)
	 {
	    for (unsigned j = 0; j < ret.columns(); j++)
	    {
	       ret[i][j] = (T) 0;
	       for (unsigned k = 0; k < _sizeM; k++)
		  ret[i][j] += _matrix[i][k] * mat[k][j];
	    }
	 }
	 return ret;
      }

      VectorN<T> operator*(const VectorN<T> &vec) const
      {
	 VectorN<T> ret(_sizeN);

	 assert(_sizeM == vec.size());

	 for (unsigned i = 0; i < _sizeN; i++)
	 {
	    ret[i] = (T) 0;
	    for (unsigned j = 0; j < _sizeM; j++)
	       ret[i] += _matrix[i][j] * vec[j];
	 }
	 return ret;
      }

      MatrixN<T> operator*(const T &scalar) const
      {
	 MatrixN<T> prod(_sizeN, _sizeM);

	 for (unsigned i = 0; i < prod.rows(); i++)
	 {
	    for (unsigned j = 0; j < prod.columns(); j++)
	       prod[i][j] = _matrix[i][j] * scalar;
	 }
	 return prod;	
      }
      
      friend MatrixN<T> operator*(const T &scalar, const MatrixN<T> &mat) 
      {
	 return mat * scalar;
      }

      MatrixN<T> operator /(const T &scalar) const
      {
	 MatrixN<T> prod(_sizeN, _sizeM);

	 for (unsigned i = 0; i < _sizeN; i++)
	 {
	    for (unsigned j = 0; j < _sizeM; j++)
	       prod[i][j] = _matrix[i][j]/scalar;
	 }
	 return prod;
      }

      MatrixN<T> operator+()
      {
	 return *this;
      }

      MatrixN<T> operator-()
      {
	 MatrixN<T> ret(_sizeN, _sizeM);

	 for (unsigned i = 0; i < ret.rows(); i++)
	 {
	    for (unsigned j = 0; j < ret.columns(); j++)
	       ret[i][j] = - _matrix[i][j];
	 }
	 
	 return ret;
      }

      MatrixN<T> &operator+=(const MatrixN<T> &mat) { return *this + mat; }
      MatrixN<T> &operator-=(const MatrixN<T> &mat) { return *this - mat; }
      MatrixN<T> &operator*=(const T &scalar) { return (*this)*scalar; }
      MatrixN<T> &operator*=(const MatrixN<T> &mat) { return (*this)*mat; }
      MatrixN<T> &operator/=(const T &scalar) { return (*this)/scalar;}

      inline unsigned rows() const { return _sizeN; }
      inline unsigned columns() const { return _sizeM; }
      
     protected:

      void init(const unsigned &n, const unsigned &m)
      {
	 _sizeN = std::min((unsigned)AL_MAX_MHEIGHT, std::max((unsigned)0, n));
	 _sizeM = std::min((unsigned)AL_MAX_MWIDTH, std::max((unsigned)0, m));

	 _matrix = (T **) malloc(_sizeN*sizeof(T *));
   
	 for (unsigned i = 0; i < _sizeN; i++)
	    _matrix[i] = (T *) malloc(_sizeM*sizeof(T));
      }

      T **_matrix;
      unsigned _sizeN, _sizeM; //N lines x M cols

   };


   template<typename T> MatrixN<T> identity(const unsigned &n)
   {
      MatrixN<T> ret(n, n);
	 
      for (unsigned i = 0; i < ret.rows(); i++)
	 ret[i][i] = (T) 1;
	 
      return ret;
   }

   //bool isInvertible();
   //virtual MatrixN<T> inverse() const;
   //virtual T determinant();

   template<typename T> MatrixN<T> transpose(const MatrixN<T> &mat) 
   {
      MatrixN<T> ret(mat.columns(), mat.rows());

      for (unsigned i = 0; i < ret.rows(); i++)
      {
	 for (unsigned j = 0; j < ret.columns(); j++)
	    ret[i][j] = mat[j][i];
      }
      return ret;
   }

   template<typename T> MatrixN<T> diagonal(const VectorN<T> &vec)
   {
      MatrixN<T> ret(vec.size(), vec.size());
      
      for (unsigned i = 0; i < ret.rows(); i++)
	 ret[i][i] = vec[i];

      return ret;
   }

   template<typename T> VectorN<T> diagonal(const MatrixN<T> &mat)
   {
      //fix this: must check for matching sizes
      assert(mat.rows() == mat.columns());

      VectorN<T> ret(mat.rows());

       for (unsigned i = 0; i < ret.size(); i++)
	 ret[i] = mat[i][i];
     
       return ret;
   }

   template<typename T> T trace(const MatrixN<T> &mat)
   {
      T ret = (T) 0;

      //fix this: must check for matching sizes
      assert(mat.rows() == mat.columns());
      
      for (unsigned i = 0; i < mat.rows(); i++)
	 ret += mat[i][i];      

      return ret;
   }



   typedef MatrixN<Real> rMatrixN;
   typedef MatrixN<Complex> cMatrixN;

   cMatrixN conjugate(const cMatrixN &mat);
   cMatrixN adjoint(const cMatrixN &mat);
}

#endif
