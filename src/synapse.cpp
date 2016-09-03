#include <synapse.h>
#include <neuron.h>

using namespace tools;
using namespace neural;
using namespace std;

synapse::synapse()
{
  _edge = NULL;
  _output = (nsignal) 0;
  _actfunc = NULL;
  _weight = (nweight) 0;
}

synapse::~synapse()
{

}

void synapse::update()
{
  nsignal ainput;
  gnode* n = _edge->inNode();

  if (n != NULL) {
    neuron* nr = (neuron*) n->getProperty();
    ainput = nr->getOutput();

    //apply the activation function
    if (_actfunc != NULL) {
      _output = _actfunc->calculate(ainput);
    } else {
      _output = ainput;
    }
  }
}


