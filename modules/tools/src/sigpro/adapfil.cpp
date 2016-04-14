#include "sigpro/adapfil.h"

using namespace std;
using namespace tools;

// Apply a filter to a signalVector
signalVector tools::filter(const signalVector &signal, signalFilter *filter)
{
   signalVector ret(signal.size(), 0);
   
   for (uint i = 0; i < signal.size(); i++)
   {
      filter->setInput(signal[i]);
      filter->update();
      ret[i] = filter->getOutput();
   }
   
   return ret;
}

//====== Least Mean Squares adaptive filter ======//
algoLMS::algoLMS(const Real &mu, firFilter *f) : firTrainer(f)
{
   _mu = mu;
}

algoLMS::~algoLMS()
{

}

// w(i) = w(i-1) + mu.u(i)*[d(i) - u(i).w(i-1)]
void algoLMS::updateWeights(const Complex &d)
{
   Complex ei = d - _f->getOutput();
 
   _f->weights = _f->weights + _mu * conjugate(_f->values) * ei;
}


//====== Normalized LMS adaptive filter ======//
algoNLMS::algoNLMS(const Real &mu, const Real &eps, firFilter *f) : firTrainer(f)
{
   _mu = mu;
   _eps = eps;
}

algoNLMS::~algoNLMS()
{

}

// w(i) = w(i-1) + mu.u(i)*[d(i) - u(i).w(i-1)]/(eps + ||u||^2)
void algoNLMS::updateWeights(const Complex &d)
{
   Complex ei = d - _f->getOutput();
   Real norma = 0;

   norma = length(_f->values);
   norma = norma*norma + _eps;

   _f->weights = _f->weights + (_mu/norma) * conjugate(_f->values) * ei;
}


//====== Recursive Least Squares adaptive filter ======//
algoRLS::algoRLS(const Real &lambda, const Real &eps, 
		 firFilter *f) : firTrainer(f), _Pmat(f->size(), f->size())
{
   _lambda = lambda;
   _eps = eps;

   _Pmat = (1.0/_eps) * identity<Complex>(f->size());
}

algoRLS::~algoRLS()
{

}

// w(i) = w(i-1) + P(i).u(i)*[d(i) - u(i).w(i-1)]
void algoRLS::updateWeights(const Complex &d)
{
   //first, lets update the matrix P
   //P(i) = [P(i-1) - (P(i-1).u(i)*.u(i).P(i-1)/lambda)/(1 + u(i).P(i-1)u(i)*/lambda)]
   
   cMatrixN u(_f->values);
   Complex temp = _lambda + _f->values*(_Pmat*conjugate(_f->values));

   _Pmat = (_Pmat - ((_Pmat*conjugate(u))*(transpose(u)*_Pmat))/temp)/_lambda; 

   //now, we update the weights
   Complex ei = d - _f->getOutput();

   _f->weights = _f->weights + _Pmat * conjugate(_f->values) * ei;
}


//====== Variable Step Size LMS adaptive filter ======//
algoVSSLMS::algoVSSLMS(const Real &mumin, const Real &mumax,
		       const Real &alpha, const Real &lambda, 
		       firFilter *f) : firTrainer(f)
{
   _mumax = mumax;
   _mumin = mumin;
   
   _alpha = alpha;
   _lambda = lambda;

   _mu = _mumin;
}

algoVSSLMS::~algoVSSLMS()
{

}

void algoVSSLMS::updateWeights(const Complex &d)
{
   Complex ei = d - _f->getOutput();

   //calculate variable mu
   _mu = _alpha*_mu + _lambda*norm(ei);

   _mu = max(min(_mu, _mumax), _mumin);
   
   _f->weights = _f->weights + _mu * conjugate(_f->values) * ei;
}


//====== Robust Variable Step Size LMS adaptive filter ======//
algoRVSLMS::algoRVSLMS(const Real &mumin, const Real &mumax,
		       const Real &alpha, const Real &lambda, 
		       const Real &beta, firFilter *f) : firTrainer(f)
{
   _mumax = mumax;
   _mumin = mumin;
   
   _alpha = alpha;
   _lambda = lambda;
   _beta = beta;

   _mu = _mumin;
   _p = 0;
   _ek = 0;
}

algoRVSLMS::~algoRVSLMS()
{

}

void algoRVSLMS::updateWeights(const Complex &d)
{
   Complex ei = d - _f->getOutput();

   //calculate p
   _p = _beta*_p + (1.0f - _beta)*(ei*_ek);

   //calculate variable mu
   _mu = _alpha*_mu + _lambda*norm(_p);
   _mu = max(min(_mu, _mumax), _mumin);

   _f->weights = _f->weights + _mu * conjugate(_f->values) * ei;   

   _ek = ei;
}


//====== Robust Variable Step Size LMS adaptive filter ======//
algoCPVSLMS::algoCPVSLMS(const Real &mumin, const Real &mumax,
		       const Real &alpha, const unsigned &period, 
		       firFilter *f) : firTrainer(f)
{
   _mumax = mumax;
   _mumin = mumin;
   
   _alpha = alpha;
   _T = period;
   _tcount = 0;

   _mu1 = _mumin;
   _mu2 = _mumax;
 
   _speedFil = new firFilter(f->size());
   
   _alms = new algoLMS(_mu2, _speedFil);

   _e1 = _e2 = 0.0;
}

algoCPVSLMS::~algoCPVSLMS()
{
   delete _speedFil;
   delete _alms;
}

void algoCPVSLMS::updateWeights(const Complex &d)
{   
   //fix this - must copy values, and output of _f
   _speedFil->setInput(_f->values[0]);
   _speedFil->update();
   
   Complex e1 = d - _f->getOutput();
   Complex e2 = d - _speedFil->getOutput();

   _e1 += norm(e1);
   _e2 += norm(e2);

   _alms->updateWeights(d);
 
   if (_tcount >= _T)
   {
      if (_e1 > _e2)
      {
	 _f->weights = _speedFil->weights;
      
	 _mu1 = (_mu1 + _mumax)*0.5;
      }
      else
      {
	 _f->weights = _f->weights + _mu1 * conjugate(_f->values) * e1;
	 
	 _mu1 = max(_alpha*_mu1, _mumin);
      }

      _tcount = 0;
      _e1 = 0.0;
      _e2 = 0.0;
   }
   else
   {
      _f->weights = _f->weights + _mu1 * conjugate(_f->values) * e1;
   }
  
   _tcount++;
}


//====== Variable Step Size NLMS adaptive filter ======//
algoVSNLMS::algoVSNLMS(const Real &mumax, const Real &alpha, 
		       const Real &epsilon, firFilter *f) : firTrainer(f), _p(f->size())

{
   _mumax = mumax;
   _alpha = alpha;
   _epsilon = epsilon;

   _mu = _mumax;
}

algoVSNLMS::~algoVSNLMS()
{

}

void algoVSNLMS::updateWeights(const Complex &d)
{
   Complex ei = d - _f->getOutput();

   Real norm_p = 0;
   Real norma = 0;
  
   norma = length(_f->values);
   norma = norma*norma + _epsilon;

   //calculate vector p
   _p = _alpha*_p + (1.0 - _alpha)*conjugate(_f->values)*ei/norma;

   norm_p = length(_p);
   norm_p = norm_p*norm_p;

   //calculate variable mu
   _mu = _mumax*norm_p/(norm_p + _epsilon);

   _f->weights = _f->weights + (_mu/norma) * conjugate(_f->values) * ei;
}
