#ifndef __SYNAPSE_H__
#define __SYNAPSE_H__

#include "graph/dedge.h"
#include "afunction.h"

namespace neural {
class synapse;
class neuron;

typedef tools::dedge<neuron*, synapse*> gedge;
typedef typename gedge::dedgeMap edgeMap;

class synapse
{
 public:

  friend class neuralNet;

  inline gedge* getEdge() const
  {
    return _edge;
  }
  inline tools::edgeId getEdgeId()
  {
    return _edge->id();
  }

  inline void setFunction(afunction* func)
  {
    _actfunc = func;
  }
  inline afunction* getFunction()
  {
    return _actfunc;
  }

  void update();
  inline nweight getWeight() const
  {
    return _weight;
  }
  inline void setWeight(const nweight& w)
  {
    _weight = w;
  }

  inline nsignal getNOutput()
  {
    return _output;
  }
  inline nsignal getOutput() const
  {
    return _weight * _output;
  }

 protected:

  synapse();
  ~synapse();

  inline void attachEdge(gedge* edge)
  {
    _edge = edge;
  }

  gedge* _edge;
  nsignal _output;
  nweight _weight;

  afunction* _actfunc;

 private:

};

}

#endif
