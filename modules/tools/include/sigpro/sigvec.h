#ifndef __SIGVEC_H__
#define __SIGVEC_H__

#include <vector>
#include "algebra/vectorn.h"

namespace tools {
//////////////////////////////////////////////////////////
/// Signal array series
//////////////////////////////////////////////////////////
/*   template<typename T> class signalArray
   {
     public:

     signalArray() : signalArray(0) {}

      signalArray(const unsigned &n)
      {
	 _numsam = n;
	 _samples.resize(_numsam);
      }

      signalArray(const unsigned &n, const T value)
      {
	 _numsam = n;
  	 _samples.resize(_numsam, value);
      }

      ~signalArray() {}

      inline vectorN<T> &operator[](const unsigned &index)
      {
	 assert(index < _numsam);
	 return _samples[index];
      }

      inline const vectorN<T> &operator[](const unsigned &index) const
      {
	 assert(index < _numsam);
	 return _samples[index];
      }

      virtual T getSample(const unsigned &index) const
      {
	 assert(index < _numsam);
	 return _samples[index];
      }

      virtual void setSample(const unsigned &index, const T &sample)
      {
	 assert(index < _numsam);
	 _samples[index] = sample;
      }

      virtual void concatenate(const signalArray<T> &sig)
      {
	 for (uint i=0; i < sig.sampleCount(); i++)
	 {
	    _samples.push_back(sig[i]);
	    _numsam++;
	 }
      }

      virtual void pushBackSample(const vectorN<T> &sample)
      {

	 vectorN<T> tmp(_numsig);

	 for (uint i=0; i < _numsig; i++)
	     tmp[i] = sample[i];

	 _samples.push_back(tmp);
      }


      inline unsigned signalCount() const { return _numsig; }
      inline unsigned sampleCount() const { return _numsam; }

     protected:
      unsigned _numsig; // number of signals
      unsigned _numsam; // number of samples
      std::vector< vectorN<T> > _samples;

   };
*/

//////////////////////////////////////////////////////////
/// Single signal series
//////////////////////////////////////////////////////////
template<typename T> class singleSignal : public vectorN<T>
{
 public:
  singleSignal(const unsigned& n, const T value = 0) : vectorN<T>(n, value) {}
  ~singleSignal() {}

  inline unsigned sampleCount() const
  {
    return vectorN<T>::_size;
  }
};


//////////////////////////////////////////////////////////
/// Array of signals (for MIMO operations)
/// n: number of signals m: number of samples
//////////////////////////////////////////////////////////
template<typename T> class multiSignal
{
 public:

  multiSignal() : multiSignal(0, 0) {}

  multiSignal(const unsigned& n, const unsigned& m, const T value = 0)
  {
    _numsig = m;
    _numsam = n;

    _samples.resize(_numsam, vectorN<T>(_numsig, value));
  }

  multiSignal(const multiSignal<T>& sig)
  {
    _numsig = sig.signalCount();
    _numsam = sig.sampleCount();

    _samples.resize(_numsam, vectorN<T>(_numsig));

    for (uint i = 0; i < _numsam; i++) {
      _samples[i] = sig[i];
    }

  }

  multiSignal(const singleSignal<T>& sig)
  {
    _numsig = 1;
    _numsam = sig.size();

    _samples.resize(_numsam, vectorN<T>(_numsig));

    for (uint i = 0; i < _numsam; i++) {
      _samples[i] = vectorN<T>(1, sig[i]);
    }
  }

  ~multiSignal() {}

  inline vectorN<T>& operator[](const unsigned& index)
  {
    assert(index < _numsam);
    return _samples[index];
  }

  inline const vectorN<T>& operator[](const unsigned& index) const
  {
    assert(index < _numsam);
    return _samples[index];
  }

  vectorN<T> getSample(const unsigned& index) const
  {
    assert(index < _numsam);
    return _samples[index];
  }

  void setSample(const unsigned& index, const vectorN<T>& sample)
  {
    assert(index < _numsam);
    assert(sample.size() == _numsig);

    for (uint i=0; i < _numsig; i++) {
      _samples[index][i] = sample[i];
    }
  }

  void concatenate(const multiSignal<T>& sig)
  {
    assert(sig.signalCount() == _numsig);

    for (uint i=0; i < sig.sampleCount(); i++) {
      vectorN<T> tmp(_numsig);

      for (uint j=0; j < _numsig; j++) {
        tmp[j] = sig[i][j];
      }

      _samples.push_back(tmp);
      _numsam++;
    }
  }

  void pushBackSample(const vectorN<T>& sample)
  {
    assert(sample.size() == _numsig);

    vectorN<T> tmp(_numsig);

    for (uint i=0; i < _numsig; i++) {
      tmp[i] = sample[i];
    }

    _samples.push_back(tmp);
    _numsam++;
  }


  inline unsigned signalCount() const
  {
    return _numsig;
  }
  inline unsigned sampleCount() const
  {
    return _numsam;
  }

 protected:
  unsigned _numsig; // number of signals
  unsigned _numsam; // number of samples
  std::vector< vectorN<T> > _samples;
};


//////////////////////////////////////////////////////////
/// Default types and auxiliary functions
//////////////////////////////////////////////////////////
typedef Real signalSample;
typedef singleSignal<signalSample> signalSeries;
typedef multiSignal<signalSample> seriesArray;
typedef vectorN<signalSample> arraySample;
}


#endif //__SIGVEC_H__
