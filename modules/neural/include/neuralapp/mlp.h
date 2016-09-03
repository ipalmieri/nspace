#ifndef __MLP_H__
#define __MLP_H__

#include "neuralnet.h"

namespace neural {

class mlpNet : public neuralNet
{
  friend class mlpTrainer;

 public:
  mlpNet(const unsigned& ninputs, const unsigned& noutputs);
  mlpNet(const std::vector<unsigned>& layers);
  ~mlpNet();

  inline unsigned layers() const
  {
    return _layers;
  }
  std::vector<neuron*> getLayer(const unsigned& index);

 protected:

  neuron* createNeuron()
  {
    return (neuron*) NULL;
  }
  void deleteNeuron(neuron* nr) {}
  void deleteNeuron(const tools::nodeId& id) {}
  void connectNeurons(neuron* nr1, neuron* nr2) {}
  void disconnectNeurons(neuron* nr1, neuron* nr2) {}

  void startNetwork(const std::vector<unsigned>& layers);

  unsigned _layers;
  std::vector< std::vector<neuron*> > _vlayers;

};
}

#endif
