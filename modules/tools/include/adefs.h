#ifndef __ADEFS_H__
#define __ADEFS_H__

#include <complex>

namespace tools
{
   typedef double Real;
   typedef double Radian;
   typedef std::complex<Real> Complex;

   inline Real realAbs(const Real &x) { return ((x) < 0.0)? (-x) : (x); }
}

#endif //__ADEFS_H__
