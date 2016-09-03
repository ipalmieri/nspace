#ifndef __ADEFS_H__
#define __ADEFS_H__

#include <cmath>
#include <complex>

#define CANNOT_COPY(class)		    \
   private:				    \
     class(const class&) = delete;	    \
     void operator=(const class &) = delete \
 

namespace tools {
typedef float Real;
typedef float Radian;
typedef std::complex<Real> Complex;
}

#endif //__ADEFS_H__
