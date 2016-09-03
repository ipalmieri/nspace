#include <ineuron.h>
#include <synapse.h>

using namespace std;
using namespace tools;
using namespace neural;

ineuron::ineuron(const groupId& gid):neuron(gid)
{
  _input = (nsignal) 0;
  _inweight = (nweight) 1.0;  // under normal ciscunstances this should remain 1.0
}

ineuron::~ineuron()
{

}

void ineuron::normalizeWeights()
{
  nweight total = (nweight) 0.0;

  for (edgeMap::const_iterator it = _node->inputEdgesBegin();
       it != _node->inputEdgesEnd(); ++it) {
    synapse* sn = (synapse*) (it->second)->getProperty();
    total += sn->getWeight();
  }
  total += _inweight;

  for (edgeMap::const_iterator it = _node->inputEdgesBegin();
       it != _node->inputEdgesEnd(); ++it) {
    synapse* sn = (synapse*) (it->second)->getProperty();
    sn->setWeight(sn->getWeight()/total);
  }

  _inweight /= total;

}

nsignal ineuron::calcWInput()
{
  nsignal wavg = (nsignal) 0.0;

  wavg = neuron::calcWInput() + _input * _inweight;

  return wavg;
}
