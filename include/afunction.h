#ifndef __AFUNCTION_H__
#define __AFUNCTION_H__

#include <cmath>
#include "adefs.h" //fix this

#define _DELTAD 0.0001
#define _DELTAI 0.0001

#include <iostream>
#include <stdlib.h>

namespace neural
{

   typedef double nsignal;
   typedef double nweight;

   //==================== BASE CLASS ====================//
   class afunction
   {
     public:
      afunction() { _isDerivable = false; } 
      virtual ~afunction() {}

      virtual nsignal calculate(const nsignal &input) = 0;
      virtual afunction *getDerivative() { return NULL; }
      virtual afunction *getPrimitive() { return NULL; }

      bool isDerivable() const { return _isDerivable; }

      nsignal calcDerivative(const nsignal &input, const nsignal &delta=_DELTAD)
      {
	 nsignal ret = 0.0;
	 
	 ret += -this->calculate(input + 2.0*delta);
	 ret += 8.0*this->calculate(input + delta);
	 ret += -8.0*this->calculate(input - delta);
	 ret += this->calculate(input - 2.0*delta);
	 
	 ret /= 12.0*delta;

	 return ret;
      }

      nsignal calcIntegral(const nsignal &a, const nsignal &b, const nsignal &delta=_DELTAI)
      {
	 nsignal sum = 0.0;
	 nsignal mult = 1.0;
	 
	 nsignal xa, xb;
	 
	 xa = a;
	 xb = b;
	 
	 if (xb < xa)
	 {
	    mult = -1.0;

	    xa = b;
	    xb = a;
	 }
	 
	 unsigned n = floor((xb - xa)/delta) + 1;
	 
	 for (unsigned i = 0; i < n; i++)
	 {
	    nsignal na = xa + ((double)i)*delta;
	    nsignal nb = (na + delta > xb)? xb : na + delta;

	    sum += (nb - na)*(this->calculate(na) + this->calculate(nb))*0.5;
	 }

	 return mult*sum;
      }

     protected:
      
      bool _isDerivable;

     private:

   };

   //pointer to function - use with care
   class pfunction : public afunction
   {
     public:
      pfunction(afunction *a) { _func = a; _isDerivable = a->isDerivable(); }
      ~pfunction() {}
      
      nsignal calculate(const nsignal &x) { return (_func)? _func->calculate(x) : 0.0f; }

     protected:
      afunction *_func;
   };

   //==================== add function ====================//
   class addFunc : public afunction
   {
     public:
      addFunc(afunction *f, afunction *g) { _f = f; _g = g; 
	 _isDerivable = (_f && _g)? (_f->isDerivable() && _g->isDerivable()) : false; }	 
      ~addFunc() { if (_f) delete _f; if (_g) delete _g; }

      nsignal calculate(const nsignal &x)
      { return (_f != NULL && _g != NULL)? _f->calculate(x) + _g->calculate(x) : 0.0f; }

      afunction *getDerivative()
      { return  (_isDerivable)? new addFunc(_f->getDerivative(), _g->getDerivative()) : NULL; }

     protected:
      afunction *_f;
      afunction *_g;
      
   };

   //==================== multiply function ====================//
   class multiplyFunc : public afunction
   {
     public:
      multiplyFunc(afunction *f, afunction *g) { _f = f; _g = g; 
	 _isDerivable = (_f && _g)? (_f->isDerivable() && _g->isDerivable()) : false; }
      ~multiplyFunc() { if (_f) delete _f; if (_g) delete _g; }

      nsignal calculate(const nsignal &x)
      { return (_f != NULL && _g != NULL)? _f->calculate(x) * _g->calculate(x) : 0.0f; }	 

      afunction *getDerivative() 
      { return  (_isDerivable)? new addFunc(new multiplyFunc(new pfunction(_f), _g->getDerivative()), \
					    new multiplyFunc(new pfunction(_g), _f->getDerivative())) : NULL; }

     protected:
      afunction *_f;
      afunction *_g;
      
   };

   //==================== composite function ====================//
   // returns f(g(x))
   class compositeFunc : public afunction
   {
     public:
      compositeFunc(afunction *f, afunction *g) { _f = f; _g = g; 
	 _isDerivable = (_f && _g)? (_f->isDerivable() && _g->isDerivable()) : false; }
      ~compositeFunc() { if (_f) delete _f; if (_g) delete _g; }

      nsignal calculate(const nsignal &x)
      { return (_f != NULL && _g != NULL)? _f->calculate(_g->calculate(x)) : 0.0f; }
      
      afunction *getDerivative()
      { return (_isDerivable)? new multiplyFunc(_f->getDerivative(), _g->getDerivative()) : NULL; }

     protected:
      afunction *_f;
      afunction *_g;
      
   };

   //==================== step function ====================//
   class stepFunction : public afunction
   {
     public:
      stepFunction(const nsignal &low=0, const nsignal &high=1) { _low = low; _high = high; }
      ~stepFunction() {}
      
      nsignal calculate(const nsignal &input) 
      {
	 if (input > 0) return (nsignal) _high;
	 return (nsignal) _low;
      }

     protected:
      nsignal _low;
      nsignal _high;

     private:

   };

   //==================== constant function ====================//
   class constFunction : public afunction
   {
     public:
      constFunction(const nsignal &k) { _k = k; _isDerivable = true; }
      ~constFunction() {}
      
      nsignal calculate(const nsignal &x=0.0f) { return _k; }
      afunction *getDerivative() { return new constFunction(0.0f); }

     protected:
      nsignal _k;
   };

   //==================== identity function ====================//
   class idFunction : public afunction
   {
     public:
      idFunction() {}
      ~idFunction() {}
      
      nsignal calculate(const nsignal &ninput) { return ninput; }
      afunction *getDerivative() { return new constFunction(1.0f); }
   };

   //==================== gaussian function ====================//
   //Gaussian distribution - parameters a,b,c must be > 0
   class gaussFunction : public afunction
   {
     public:
      gaussFunction(const double &avg, const double &stdev) { setParameters(avg, stdev); }
      ~gaussFunction() {}
      
      nsignal calculate(const nsignal &x){ return _a * exp( -pow(x - _b, 2) / (2 * pow(_c, 2)) ); }

      inline void setParameters(const double &avg, const double &stdev)
      {
	 _a = 1.0 / (stdev * sqrt(2.0 * M_PI));
	 _b = avg;
	 _c = stdev;
      }

      inline double avg() { return _b; }
      inline double stdev() { return _c; }
      
     protected:
      double _a, _b, _c;
   };

   //==================== exponential ====================//
   class expFunction : public afunction
   {
     public:
     expFunction() : afunction() {}
      ~expFunction() {}

      nsignal calculate(const nsignal &x) { return exp(x); }
      afunction *getDerivative() { return new expFunction(); }
   };


   //==================== logistic function ====================//
   class logisticFunction : public afunction
   {
     public:
     logisticFunction() : afunction()
      {	 A = 0.0f; K = 1.0f; B = 1.0f; v = 1.0f; 
	 Q = 1.0f; M = 0.0f; _isDerivable = true; }

      nsignal A, K, B, v, Q, M;

      nsignal calculate(const nsignal &x) 
      { return A + (K - A)/pow(1.0f + Q*exp(-B*(x - M)), 1.0f/v); }
   };
   
   //==================== H-sec function ====================//
   class sechFunction : public afunction
   {
     public:
      sechFunction() { _isDerivable = true; }

      nsignal calculate(const nsignal &x) { return 1.0/cosh(x); }
      
   };

   //==================== x^2 function ====================//
   class squareFunction : public afunction
   {
     public:
      squareFunction() { _isDerivable = true; }

      nsignal calculate(const nsignal &x) { return x*x; }
   
      afunction *getDerivative() 
      { return new multiplyFunc(new constFunction(2.0f), new idFunction()); }
      
   }; 

   //==================== H-tan function ====================//
   class tanhFunction : public afunction
   {
     public:
     tanhFunction() : afunction() { _isDerivable = true; }

      nsignal calculate(const nsignal &x) { return tanh(x); }

      afunction *getDerivative() { return new compositeFunc(new squareFunction(), \
							    new sechFunction()); }
   };

}

#endif
