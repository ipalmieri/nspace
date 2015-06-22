#include "algebra/vectorn.h"

using namespace tools;
using namespace std;

Real tools::length(const rVectorN &vec) 
{
   Real ret = 0.0f;

   for (unsigned i = 0; i < vec.size(); i++)
      ret += vec[i]*vec[i];

   return sqrt(ret);
}

Real tools::length(const cVectorN &vec) 
{
   Real ret = 0.0f;

   for (unsigned i = 0; i < vec.size(); i++)
      ret += norm(vec[i]);
      
   return sqrt(ret);
}

cVectorN tools::conjugate(const cVectorN &vec)
{
   cVectorN ret(vec.size());

   for (unsigned i = 0; i < ret.size(); i++)
      ret[i] = conj(vec[i]);
   
   return ret;
}

rVectorN tools::normalize(const rVectorN &vec)
{
   Real len = length(vec);

   return (len > 0.0)? vec/len : vec;
}

cVectorN tools::normalize(const cVectorN &vec)
{
   Real len = length(vec);

   return (len > 0.0)? vec/len : vec;
}
