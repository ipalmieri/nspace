#include "neuralapp/kneuron.h"
#include <cmath>

using namespace neural;
using namespace tools;
using namespace std;

kneuron::kneuron(const groupId& gid) : neuron(gid)
{
  _hits = 0;

  _pos.x = 0.0;
  _pos.y = 0.0;
  _pos.z = 0.0;
}

kneuron::~kneuron()
{

}

//must return the euclidean difference weight to input
nsignal kneuron::calcWInput()
{
  nsignal ret = (nsignal) 0.0;
  synapse* tsyn = (synapse*) NULL;
  nsignal diff = 0.0;

  for (edgeMap::const_iterator it = _node->inputEdgesBegin();
       it != _node->inputEdgesEnd(); ++it) {
    tsyn = (synapse*) (it->second)->getProperty();
    diff = tsyn->getNOutput() - tsyn->getWeight();
    ret += diff*diff;
  }

  ret = sqrt(ret);

  return ret;
}

vector<nweight> kneuron::getInputWeights() const
{
  vector<nweight> ret;

  vector<synapse*> tsyn = getInputSynapses();

  for (unsigned i = 0; i < tsyn.size(); i++) {
    ret.push_back(tsyn[i]->getWeight());
  }

  return ret;
}
