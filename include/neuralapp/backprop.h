#ifndef __BACKPROP_H__
#define __BACKPROP_H__

#include "neuralnet.h"

namespace neural {
//backNeuron - backward copy of a neuron
class backNeuron : public ineuron
{

  friend class backNet;

 public:

  nsignal acc;

  neuron* getNeuron()
  {
    return _fneuron;
  }
  void update();

 protected:
  backNeuron(neuron* nr, const groupId& gid=0);
  ~backNeuron() {}

  neuron* _fneuron;
};


//backSynapse - backward copy of a synapse
class backSynapse : public synapse
{
  friend class backNet;

 public:

  nsignal acc;

  synapse* getSynapse()
  {
    return _fsynapse;
  }

 protected:
  backSynapse(synapse* sn);
  ~backSynapse() {}

  synapse* _fsynapse;

};


//backward neural network
class backNet : public neuralNet
{
 public:
  backNet(neuralNet* net);
  ~backNet();

 protected:

  void cloneNet(neuralNet* net);
  backNeuron* createNeuron(neuron* nr);

  neuron* createNeuron()
  {
    return NULL;
  }
  void deleteNeuron(neuron* nr) {}
  void deleteNeuron(const tools::nodeId& id) {}
  void connectNeurons(neuron* nr1, neuron* nr2) {}
  void disconnectNeurons(neuron* nr1, neuron* nr2) {}

};

//backProp trainer
class backProp
{
 public:
  backProp(neuralNet* net, const nsignal& neta);
  ~backProp();

  void addData(tools::vectorN<nsignal>& input, tools::vectorN<nsignal>& output);

  void stepEpoch();
  void trainLoop(const unsigned& maxiter);

  neuralNet* getBackNet()
  {
    return (neuralNet*)_backNet;
  }

  tools::Real netError();

  nsignal eta;

 protected:

  void accumulateErrors();

  neuralNet* _forwardNet;
  backNet* _backNet;

  afunction* _errorFunc;
  afunction* _derFunc;

  std::vector< tools::vectorN<nsignal> > _inData;
  std::vector< tools::vectorN<nsignal> > _ouData;
};
}

#endif
