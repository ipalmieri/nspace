#include "sigpro/filter.h"

using namespace std;
using namespace tools;

firFilter::firFilter(const unsigned& ninputs)
  : weights(ninputs),
    values(ninputs)
{
  _length = ninputs;

  _inval = 0;
  _ouval = 0;
}

firFilter::~firFilter()
{

}

void firFilter::setInput(const signalSample& val)
{
  _inval = val;
}

void firFilter::update()
{
  for (unsigned i = _length - 1; i > 0; i--) {
    values[i] = values[i - 1];
  }

  values[0] = _inval;

  _ouval = weights*values;
}

iirFilter::iirFilter(const unsigned& ninputs, const unsigned& noutputs)
  : inWeights(ninputs),
    ouWeights(noutputs),
    _inValues(ninputs),
    _ouValues(noutputs)
{
  _inLength = _inValues.size();
  _ouLength = _ouValues.size();

  _inval = 0.0;

  ouWeights[0] = 1.0;
}

iirFilter::~iirFilter()
{

}

void iirFilter::update()
{
  signalSample outp = 0;

  for (unsigned i = _inLength - 1; i > 0; i--) {
    _inValues[i] = _inValues[i - 1];
  }

  for (unsigned i = _ouLength - 1; i > 0; i--) {
    _ouValues[i] = _ouValues[i - 1];
  }


  _inValues[0] = _inval;

  outp = _inValues * inWeights;

  for (unsigned i = _ouLength - 1; i > 0; i--) {
    outp -= _ouValues[i] * ouWeights[i];
  }

  assert(abs(ouWeights[0]) > 0.0);

  outp = outp/ouWeights[0];

  _ouValues[0] = outp;
}

// Apply a filter to a signalVector
signalSeries tools::filter(const signalSeries& signal, signalFilter& filter)
{
  signalSeries ret(signal.size(), 0);

  for (uint i = 0; i < signal.sampleCount(); i++) {
    filter.setInput(signal[i]);
    filter.update();
    ret[i] = filter.getOutput();
  }

  return ret;
}

seriesArray tools::filter(const seriesArray& arr, mimoFilter& filter)
{
  seriesArray ret(arr.sampleCount(), arr.signalCount());

  arraySample tmp(arr.signalCount());

  for (uint i = 0; i < arr.sampleCount(); i++) {
    tmp = arr.getSample(i);

    filter.setInput(tmp);

    filter.update();

    tmp = filter.getOutput();

    ret.setSample(i, tmp);
  }

  return ret;
}
