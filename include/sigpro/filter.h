#ifndef __FILTER_H__
#define __FILTER_H__

#include "sigpro/sigvec.h"
#include "algebra.h"

namespace tools
{
   ////////////////////////////////////////////////////////
   /// filter base class
   ////////////////////////////////////////////////////////
   class signalFilter
   {
     public:
      signalFilter();
      virtual ~signalFilter();
      
      virtual void update() = 0;
      virtual void setInput(const Complex &val) = 0;
      virtual Complex getOutput() = 0;
   };


   ////////////////////////////////////////////////////////
   /// FIR filter class
   ////////////////////////////////////////////////////////
   class firFilter : public signalFilter
   {
     public:

      firFilter(const unsigned &ninputs);
      ~firFilter();

      void update();
      void setInput(const Complex &val);
      inline Complex getOutput() { return _ouval; }
      inline unsigned size() { return _length; } 

      cVectorN weights;
      cVectorN values;


     protected:

      unsigned _length;
      Complex _inval;
      Complex _ouval;
   };


   ////////////////////////////////////////////////////////
   /// IIR filter class
   ////////////////////////////////////////////////////////
   class iirFilter : public signalFilter
   {
     public:

      iirFilter(const unsigned &ninputs, const unsigned &noutputs);
      ~iirFilter();

      void update();
      void setInput(const Complex &val) { _inval = val; }
      inline Complex getOutput() { return _ouValues[0]; }
      inline unsigned inputSize() { return _inLength; }
      inline unsigned outputSize() { return _ouLength; }
      cVectorN &inValues() { return _inValues; }

      cVectorN inWeights;
      cVectorN ouWeights;


     protected:
      
      unsigned _inLength;
      unsigned _ouLength;
      cVectorN _inValues;
      cVectorN _ouValues;
      Complex _inval;
   };


   ////////////////////////////////////////////////////////
   /// filter function
   ////////////////////////////////////////////////////////
   signalVector filter(const signalVector &signal, signalFilter *filter);
  
}

#endif //__FILTER_H__
