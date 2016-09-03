#ifndef __ADAPFIL_H__
#define __ADAPFIL_H__

#include "sigpro/filter.h"
#include "algebra.h"

namespace tools {
////////////////////////////////////////////////////////
/// Base class for a FIR Filter Adaptive Trainer
////////////////////////////////////////////////////////
class firTrainer
{
 public:

  firTrainer(firFilter* f)
  {
    _f = f;
  }
  virtual ~firTrainer() {}
  virtual void updateWeights(const signalSample& d) = 0;

 protected:

  firFilter* _f; ///< filter to be updated
};


////////////////////////////////////////////////////////
/// LMS algorithm firTrainer
////////////////////////////////////////////////////////
class algoLMS : public firTrainer
{
 public:

  algoLMS(const Real& mu, firFilter* f);
  ~algoLMS();
  void updateWeights(const signalSample& d);


 protected:

  Real _mu;
};


////////////////////////////////////////////////////////
/// Normalized LMS algorithm firTrainer
////////////////////////////////////////////////////////
class algoNLMS : public firTrainer
{
 public:

  algoNLMS(const Real& mu, const Real& eps, firFilter* f);
  ~algoNLMS();
  void updateWeights(const signalSample& d);


 protected:

  Real _eps;
  Real _mu;
};


////////////////////////////////////////////////////////
/// RLS algorithm firTrainer
////////////////////////////////////////////////////////
class algoRLS : public firTrainer
{
 public:

  algoRLS(const Real& lambda, const Real& eps, firFilter* f);
  ~algoRLS();
  void updateWeights(const signalSample& d);


 protected:

  matrixN<signalSample> _Pmat;
  Real _eps;
  Real _lambda;
};


////////////////////////////////////////////////////////
/// VSS-LMS algorithm firTrainer
////////////////////////////////////////////////////////
class algoVSSLMS : public firTrainer
{
 public:

  algoVSSLMS(const Real& mumin, const Real& mumax,
             const Real& alpha, const Real& lambda, firFilter* f);
  ~algoVSSLMS();
  void updateWeights(const signalSample& d);


 protected:

  Real _mumax;
  Real _mumin;
  Real _alpha;
  Real _lambda;
  Real _mu;
};


////////////////////////////////////////////////////////
/// RVS-LMS algorithm firTrainer
////////////////////////////////////////////////////////
class algoRVSLMS : public firTrainer
{
 public:

  algoRVSLMS(const Real& mumin, const Real& mumax,
             const Real& alpha, const Real& lambda,
             const Real& beta, firFilter* f);
  ~algoRVSLMS();
  void updateWeights(const signalSample& d);


 protected:

  Real _mumax;
  Real _mumin;
  Real _alpha;
  Real _lambda;
  Real _beta;
  Real _mu;
  signalSample _p, _ek;
};


////////////////////////////////////////////////////////
/// CPVS-LMS algorithm firTrainer
////////////////////////////////////////////////////////
class algoCPVSLMS : public firTrainer
{
 public:

  algoCPVSLMS(const Real& mumin, const Real& mumax,
              const Real& alpha, const unsigned& period,
              firFilter* f);
  ~algoCPVSLMS();
  void updateWeights(const signalSample& d);


 protected:

  Real _mumax;
  Real _mumin;
  Real _alpha;
  Real _mu1;
  Real _mu2;
  unsigned _T;
  unsigned _tcount;
  firFilter* _speedFil;
  algoLMS* _alms;
  Real _e1;
  Real _e2;
};


////////////////////////////////////////////////////////
/// VS-NLMS algorithm firTrainer
////////////////////////////////////////////////////////
class algoVSNLMS : public firTrainer
{
 public:

  algoVSNLMS(const Real& mumax, const Real& alpha,
             const Real& epsilon, firFilter* f);
  ~algoVSNLMS();
  void updateWeights(const signalSample& d);

 protected:

  Real _mumax;
  Real _alpha;
  Real _epsilon;
  Real _mu;
  vectorN<signalSample> _p;
};
}

#endif //__ADAPFIL_H__
