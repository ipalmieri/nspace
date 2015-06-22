#include "neuralapp/mlp.h"

using namespace std;
using namespace tools;
using namespace neural;

mlpNet::mlpNet(const unsigned &ninputs, const unsigned &noutputs) : neuralNet() 
{
   startNetwork(vector<unsigned>(ninputs, noutputs));
}
      
mlpNet::mlpNet(const std::vector<unsigned> &layers)
{
   startNetwork(layers);
}      

mlpNet::~mlpNet()
{

}

void mlpNet::startNetwork(const std::vector<unsigned> &layers)
{
   neuron *tempnr;
   vector<neuron *> lastlayer, nextlayer;
   
   _layers = 0;

   afunction *func = new tanhFunction();
   _functions.push_back(func);
   afunction *ifunc = new idFunction();
   _functions.push_back(ifunc);


   if (layers.size() > 0)
   {
      //create and sets inputs neurons
      for (unsigned i = 0; i < layers[0]; i++)
      {
	 tempnr = (neuron *) neuralNet::createInput();
	 tempnr->setFunction(ifunc);

	 lastlayer.push_back(tempnr);

	 _vlayers.push_back(lastlayer);
      }
      
      //create all other layers
      for (unsigned i = 1; i < layers.size(); i++)
      {
	 nextlayer.clear();

	 for (unsigned j = 0; j < layers[i]; j++)
	 {
	    tempnr = (neuron *) neuralNet::createNeuron();
	    tempnr->setFunction(func);
	 
	    for (unsigned k = 0; k < lastlayer.size(); k++)
	    {
	       neuralNet::connectNeurons(tempnr, lastlayer[k]);
	    }
	    nextlayer.push_back(tempnr);
	 }
	 
	 if (layers[i] > 0)
	 {
	    lastlayer = nextlayer;
	    _layers++;

	    _vlayers.push_back(nextlayer);
	 }
      }

      //set outputs neurons
      for (unsigned k = 0; k < lastlayer.size(); k++)
      {
	 _ovars.push_back(lastlayer[k]);
      } 
   }
}

vector<neuron *> mlpNet::getLayer(const unsigned &index)
{
   vector<neuron *> ret;

   if (index < _vlayers.size())
   {
      ret = _vlayers[index];
   }
   
   return ret;
}

