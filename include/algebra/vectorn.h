#ifndef __VECTORN_H__
#define __VECTORN_H__

#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <cassert>
#include "adefs.h"

#define AL_MAX_VSIZE 104857600

namespace tools
{
   //////////////////////////////////////////////
   /// VectorN is a multi length vector type
   /// for hilbert space algebra
   //////////////////////////////////////////////
   template<typename T> class VectorN 
   {
     public:
      
      explicit VectorN(const unsigned &n, const T &value=0)
      {
	 _vector = NULL;
	 _size = 0;

	 init(n);
	 
	 for (unsigned i = 0; i < _size; i++) 
	    _vector[i] = value;
      } 
      
      VectorN(const VectorN<T> &vec)
      {
	 init(vec._size);
	 
	 for (unsigned i = 0; i < _size; i++) 
	    _vector[i] = vec[i];
      }

      virtual ~VectorN() 
      { 
	 if (_vector)
	    free(_vector); 
      }
      
      inline unsigned size() const { return _size; }

      inline T &operator[](const unsigned &index) 
      {
	 assert(index < _size);
	 return _vector[index]; 
      }
      
      inline T operator[](const unsigned &index) const 
      { 
	 assert(index < _size);
	 return _vector[index]; 
      }

      VectorN<T> &operator=(const VectorN<T> &vec) 
      {
	 if (_size != vec.size())
	    init(vec._size);

	 for (unsigned i = 0; i < _size; i++)
	     _vector[i] = vec[i];
	 
	 return (*this);
      }
      
      VectorN<T> operator+(const VectorN<T> &vec) const
      {
	 VectorN<T> sum(_size);

	 assert(_size == vec.size());

	 for (unsigned i = 0; i < _size; i++)
	        sum[i] = _vector[i] + vec[i];
	 
	 return sum;
      }

      VectorN<T> operator-(const VectorN<T> &vec) const
      {
	 VectorN<T> sum(_size);

	 assert(_size == vec.size());

	 for (unsigned i = 0; i < _size; i++)
	        sum[i] = _vector[i] - vec[i];
	 
	 return sum;
      }

      //dot product
      T operator*(const VectorN<T> &vec) const
      {
	 T ret = (T) 0;

	 assert(_size == vec.size());

	 for (unsigned i = 0; i < _size; i++)
	    ret += _vector[i]*vec[i];

	 return ret;
      }

      VectorN<T> operator*(const T &scalar) const
      {
	 VectorN<T> prod(_size);

	 for (unsigned i = 0; i < _size; i++)
	    prod[i] = _vector[i]*scalar;

	 return prod;	
      }
      
      friend VectorN<T> operator*(const T &scalar, const VectorN<T> &vec) 
      {
	 return vec*scalar;
      }

      VectorN<T> operator/(const T &scalar) const
      {
	 VectorN<T> prod(_size);

	 for (unsigned i = 0; i < _size; i++)
	    prod[i] = _vector[i]/scalar;

	 return prod;
      }


      VectorN<T> operator+()
      {
	 return *this;
      }

      VectorN<T> operator-()
      {
	 VectorN<T> ret(_size);

	 for (unsigned i = 0; i < ret.size(); i++)
	    ret[i] = - _vector[i];
	 
	 return ret;
      }

      VectorN<T> &operator+=(const VectorN<T> &vec) { return *this + vec; }
      VectorN<T> &operator-=(const VectorN<T> &vec) { return *this - vec; }
      VectorN<T> &operator*=(const T &scalar) { return (*this)*scalar; }
      VectorN<T> &operator*=(const VectorN<T> &vec) { return (*this)*vec; }
      VectorN<T> &operator/=(const T &scalar) { return (*this)/scalar; }
      

     protected: 

      void init(const unsigned &n)
      {
	 unsigned nsize = (unsigned) std::min((unsigned)AL_MAX_VSIZE, 
				     std::max((unsigned)0, n));
	 
	 T *tvec = (T *) realloc(_vector, nsize*sizeof(T));

	 if (tvec) 
	 {
	    _vector = tvec;
	    _size = nsize;
	 }
      } 

    
      T *_vector;
      unsigned _size; //N elements
   };



   typedef VectorN<Real> rVectorN;
   typedef VectorN<Complex> cVectorN;

   //length (norm) of the vector
   Real length(const rVectorN &vec); 
   Real length(const cVectorN &vec); 
   inline Real norm(const rVectorN &vec) { return length(vec); }
   inline Real norm(const cVectorN &vec) { return length(vec); }
   cVectorN conjugate(const cVectorN &vec);
   rVectorN normalize(const rVectorN &vec);
   cVectorN normalize(const cVectorN &vec);

}

#endif
