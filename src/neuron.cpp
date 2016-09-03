#include <neuron.h>
#include <synapse.h>

using namespace neural;
using namespace tools;
using namespace std;

neuron::neuron(const groupId& gid)
{
  _node = NULL;
  _gid = gid;
  _actfunc = NULL;
  _output = (nsignal) 0;
  _w0bias = (nweight) 0;

}

neuron::~neuron()
{

}

void neuron::normalizeWeights()
{
  nweight total = (nweight) 0.0;

  for (edgeMap::const_iterator it = _node->inputEdgesBegin();
       it != _node->inputEdgesEnd(); ++it) {
    synapse* sn = (synapse*) (it->second)->getProperty();
    total += sn->getWeight();
  }

  for (edgeMap::const_iterator it = _node->inputEdgesBegin();
       it != _node->inputEdgesEnd(); ++it) {
    synapse* sn = (synapse*) (it->second)->getProperty();
    sn->setWeight(sn->getWeight()/total);
  }

}

void neuron::update()
{
  nsignal ainput;

  //get weighted average of inputs;
  ainput = calcWInput();

  //apply the activation function
  if (_actfunc != NULL) {
    _output = _actfunc->calculate(ainput);
  }
}

nsignal neuron::calcWInput()
{
  nsignal wavg = (nsignal) 0.0;

  for (edgeMap::const_iterator it = _node->inputEdgesBegin();
       it != _node->inputEdgesEnd(); ++it) {
    wavg += ((synapse*) (it->second)->getProperty())->getOutput();
  }

  wavg += _w0bias;

  return wavg;
}

vector<synapse*> neuron::getInputSynapses() const
{
  vector<synapse*> ret;

  for (edgeMap::const_iterator it = _node->inputEdgesBegin();
       it != _node->inputEdgesEnd(); ++it) {
    ret.push_back((synapse*)(it->second)->getProperty());
  }

  return ret;
}

vector<synapse*> neuron::getOutputSynapses() const
{
  vector<synapse*> ret;

  for (edgeMap::const_iterator it = _node->outputEdgesBegin();
       it != _node->outputEdgesEnd(); ++it) {
    ret.push_back((synapse*)(it->second)->getProperty());
  }

  return ret;
}
