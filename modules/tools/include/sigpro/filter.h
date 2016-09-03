#ifndef __FILTER_H__
#define __FILTER_H__

#include "sigpro/sigvec.h"
#include "algebra.h"

namespace tools {
////////////////////////////////////////////////////////
/// filter base class
////////////////////////////////////////////////////////
class signalFilter
{
 public:
  signalFilter();
  virtual ~signalFilter();

  virtual void update() = 0;
  virtual void setInput(const signalSample& val) = 0;
  virtual signalSample getOutput() = 0;
};

class mimoFilter
{
 public:
  mimoFilter();
  virtual ~mimoFilter() = 0;

  virtual void update() = 0;
  virtual void setInput(const vectorN<signalSample>& array) = 0;
  virtual vectorN<signalSample> getOutput() = 0;
};

////////////////////////////////////////////////////////
/// FIR filter class
////////////////////////////////////////////////////////
class firFilter : public signalFilter
{
 public:

  firFilter(const unsigned& ninputs);
  ~firFilter();

  void update();
  void setInput(const signalSample& val);
  inline signalSample getOutput()
  {
    return _ouval;
  }
  inline unsigned size()
  {
    return _length;
  }

  vectorN<signalSample> weights;
  vectorN<signalSample> values;


 protected:

  unsigned _length;
  signalSample _inval;
  signalSample _ouval;
};


////////////////////////////////////////////////////////
/// IIR filter class
////////////////////////////////////////////////////////
class iirFilter : public signalFilter
{
 public:

  iirFilter(const unsigned& ninputs, const unsigned& noutputs);
  ~iirFilter();

  void update();
  void setInput(const signalSample& val)
  {
    _inval = val;
  }
  inline signalSample getOutput()
  {
    return _ouValues[0];
  }
  inline unsigned inputSize()
  {
    return _inLength;
  }
  inline unsigned outputSize()
  {
    return _ouLength;
  }

  vectorN<signalSample> inWeights;
  vectorN<signalSample> ouWeights;

 protected:

  unsigned _inLength;
  unsigned _ouLength;
  vectorN<signalSample> _inValues;
  vectorN<signalSample> _ouValues;
  signalSample _inval;
};


////////////////////////////////////////////////////////
/// filter function
////////////////////////////////////////////////////////
signalSeries filter(const signalSeries& signal, signalFilter& filter);
seriesArray filter(const seriesArray& array, mimoFilter& filter);
}

#endif //__FILTER_H__
