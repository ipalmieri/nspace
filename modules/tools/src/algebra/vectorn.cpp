#include "algebra/vectorn.h"

using namespace tools;
using namespace std;


Real tools::length(const rvectorN& vec)
{
  Real ret = 0.0f;

  for (unsigned i = 0; i < vec.size(); i++) {
    ret += vec[i]*vec[i];
  }

  return sqrt(ret);
}

Real tools::length(const cvectorN& vec)
{
  Real ret = 0.0f;

  for (unsigned i = 0; i < vec.size(); i++) {
    ret += norm(vec[i]);
  }

  return sqrt(ret);
}

cvectorN tools::conjugate(const cvectorN& vec)
{
  cvectorN ret(vec.size());

  for (unsigned i = 0; i < ret.size(); i++) {
    ret[i] = conj(vec[i]);
  }

  return ret;
}

rvectorN tools::conjugate(const rvectorN& vec)
{
  rvectorN ret = vec;
  return ret;
}

rvectorN tools::normalize(const rvectorN& vec)
{
  Real len = length(vec);

  return (len > 0.0)? vec/len : vec;
}

cvectorN tools::normalize(const cvectorN& vec)
{
  Real len = length(vec);

  return (len > 0.0)? vec/len : vec;
}
