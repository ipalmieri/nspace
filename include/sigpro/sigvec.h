#ifndef __SIGVEC_H__
#define __SIGVEC_H__

#include "algebra/vectorn.h"

namespace tools
{
   class signalVector : public cVectorN
   {
     public:
     signalVector(const unsigned &n, const Complex value = 0.0) : cVectorN(n, value) {}
      ~signalVector();

     protected:

   };
}

#endif //__SIGVEC_H__
