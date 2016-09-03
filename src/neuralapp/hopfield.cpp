#include "neuralapp/hopfield.h"

using namespace std;
using namespace tools;
using namespace neural;

hopfieldNet::hopfieldNet(const unsigned int& ncount)
{
  afunction* func = NULL;

  if (ncount > 0) {
    func = (afunction*) new stepFunction(-1, +1);
    _functions.push_back(func);
  }

  // inputs are also outputs in a Hopfield network
  for (unsigned int i = 0; i < ncount; i++) {
    ineuron* nr = new hneuron();

    attachNeuron(nr);

    _ivars.push_back(nr);
    _ovars.push_back(nr);

    nr->setFunction(func);
  }

  // build the Hopfield structure
  for (unsigned int i = 0; i < _ivars.size(); i++) {

    //conventional inputs should be neglected - outputs are the inputs
    _ivars[i]->setInputWeight(0.0f);

    for (unsigned int j = 0; j < _ivars.size(); j++) {
      if (i != j) {
        neuralNet::connectNeurons(_ivars[i], _ivars[j]);
      }

    }
  }

}

hopfieldNet::~hopfieldNet()
{

}

void hopfieldNet::addImage()
{
  for (edgeMap::const_iterator it = _graph->edgeMapBegin();
       it != _graph->edgeMapEnd(); ++it) {
    synapse* sn = (synapse*) it->second->getProperty();
    neuron* inr = (neuron*) it->second->inNode()->getProperty();
    neuron* onr = (neuron*) it->second->outNode()->getProperty();

    sn->setWeight(sn->getWeight() + inr->getOutput()*onr->getOutput());
  }
}

void hopfieldNet::setInput(const std::vector<nsignal>& insignal)
{
  if (insignal.size() == _ivars.size()) {
    for (unsigned int i = 0; i < _ivars.size(); i++) {
      _ivars[i]->setInput(insignal[i]);  //should copy input to output
    }
  }
}

Real hopfieldNet::energy()
{
  Real ener = 0.0f;

  for (edgeMap::const_iterator it = _graph->edgeMapBegin();
       it != _graph->edgeMapEnd(); ++it) {
    synapse* sn = (synapse*) it->second->getProperty();
    neuron* inr = (neuron*) it->second->inNode()->getProperty();
    neuron* onr = (neuron*) it->second->outNode()->getProperty();

    ener += -0.5*sn->getWeight()*inr->getOutput()*onr->getOutput();
  }

  for (nodeMap::const_iterator it = _graph->nodeMapBegin();
       it != _graph->nodeMapEnd(); ++it) {
    neuron* nr = (neuron*) it->second->getProperty();

    ener += nr->getWeight()*nr->getOutput();
  }

  return ener;
}
