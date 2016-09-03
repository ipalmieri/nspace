#ifndef __GATRAINER_H__
#define __GATRAINER_H__

#include "neuralnet.h"
#include "genetic.h"

namespace neural {
//neural net weights as genes for ga
class gaWeight : public advtec::gene
{
 public:
  gaWeight(synapse* syn)
  {
    _synapse = syn;
  }
  ~gaWeight() {}

  void mutate(const double& rate);
  gene* createCopy();
  void initRand(const double& start, const double& end);

  void apply()
  {
    if (_synapse) {
      _synapse->setWeight(value);
    }
  }
  synapse* getSynapse()
  {
    return _synapse;
  }

  nweight value;

 protected:

  synapse* _synapse;
};


//neural net weightsbias as genes for ga
class gaBias : public advtec::gene
{
 public:
  gaBias(neuron* nr)
  {
    _neuron = nr;
  }
  ~gaBias() {}

  void mutate(const double& rate);
  gene* createCopy();
  void initRand(const double& start, const double& end);

  void apply()
  {
    if (_neuron) {
      _neuron->setWeight(value);
    }
  }
  neuron* getNeuron()
  {
    return _neuron;
  }

  nweight value;

 protected:

  neuron* _neuron;
};


//struct used to sort solutions
class chromEval
{
 public:
  nsignal error;
  advtec::chromossome* solution;

  bool operator <(const chromEval& b)
  {
    return (this->error < b.error);
  }
  bool operator >(const chromEval& b)
  {
    return (this->error > b.error);
  }
};


//neural net weight gaTrainer
class gaNetTrainer : public advtec::gaMethod
{
 public:
  gaNetTrainer(neuralNet* net, const unsigned& pinit);
  virtual ~gaNetTrainer();

  void addData(tools::vectorN<nsignal>& input, tools::vectorN<nsignal>& output);
  void trainLoop(const unsigned& maxgenerations);

  void setSolution(advtec::chromossome* chr);
  advtec::chromossome* getSolution(const unsigned& index);
  tools::Real netError()
  {
    return evalError(_population[0]);
  }

  void printSolutions();

 protected:

  void init(const unsigned& pinit);

  void evalPopulation();

  nsignal evalError(advtec::chromossome* chr);
  tools::Real outputError(tools::vectorN<nsignal>& ynet);

  neuralNet* _net;

  // fix this: use multiSignal
  std::vector< tools::vectorN<nsignal> > _inData;
  std::vector< tools::vectorN<nsignal> > _ouData;

};

}

#endif
