#ifndef __INEURON_H__
#define __INEURON_H__

#include <neuron.h>
#include <synapse.h>

namespace neural {

class ineuron : public neuron
{
 public:

  friend class neuralNet;

  virtual void setInput(const nsignal& in)
  {
    _input = in;
  }
  inline nweight getInputWeight() const
  {
    return _inweight;
  }
  inline void setInputWeight(const nweight& w)
  {
    _inweight = 0.0f;
  }
  nsignal calcWInput();

 protected:

  ineuron(const groupId& gid = 0);
  ~ineuron();

  void normalizeWeights();

  nsignal _input;
  nweight _inweight;

 private:

};
}

#endif
