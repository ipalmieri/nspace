#include "neuralapp/backprop.h"
#include <map>

using namespace std;
using namespace tools;
using namespace neural;

backNeuron::backNeuron(neuron *nr, const groupId &gid) : ineuron(gid)
{
   //assert nr != NULL here
   _fneuron = nr;

   acc = 0.0f;
}

void backNeuron::update()
{
   nsignal ainput;

   ainput = calcWInput();

   if (_actfunc != NULL && _fneuron != NULL)
   {
      _output = _actfunc->calculate(_fneuron->calcWInput())*ainput;
   }
}

backSynapse::backSynapse(synapse *sn) : synapse()
{
   //fix this assert sn != NULL here
   _fsynapse = sn;

   _weight = sn->getWeight();
   
   acc = 0.0f;
}

backNet::backNet(neuralNet *net)
{
   cloneNet(net);
}

void backNet::cloneNet(neuralNet *net)
{
   agraph *graph = net->getGraph();
   map<neuron *, backNeuron *> nrmap;
   map<afunction *, afunction *> derivatives;

   //creates backNeurons
   for (nodeMap::const_iterator it = graph->nodeMapBegin(); it != graph->nodeMapEnd(); ++it)
   {
      neuron *nr = (neuron *) (it->second)->getProperty();
      backNeuron *bn = createNeuron(nr);
      
      //check if it is output
      bool isOutput = false;
      for (unsigned i = 0; i < net->outputSize(); i++)
      {
	 if (net->getOutputNeuron(i) == nr)
	    isOutput = true;
      }
      if (isOutput) _ivars.push_back(bn);

      //check if it is input
      bool isInput = false;
      for (unsigned i = 0; i < net->inputSize(); i++)
      {
	 if (net->getInputNeuron(i) == nr)
	    isInput = true;
      }
      if (isInput) _ovars.push_back(bn);      
      
      //sets activation func
      afunction *actf = nr->getFunction();
      if (actf != NULL)
      {
	 map<afunction *, afunction *>::iterator it = derivatives.find(actf);
	 if (it == derivatives.end())
	 {
	    derivatives[actf] = actf->getDerivative();
	    _functions.push_back(derivatives[actf]);
	 }
      }

      bn->setFunction(derivatives[actf]);
      nrmap[nr] = bn;
   }

   //creates backSynapses
   for (edgeMap::const_iterator it = graph->edgeMapBegin(); it != graph->edgeMapEnd(); ++it)
   {
      neuron *inr = (neuron *)it->second->inNode()->getProperty();
      neuron *onr = (neuron *)it->second->outNode()->getProperty();
      
      backNeuron *bi = nrmap[inr];
      backNeuron *bo = nrmap[onr];

      //conect backNeurons
      synapse *sn = (synapse *)it->second->getProperty();
      
      backSynapse *bn = new backSynapse(sn);	 
            
      attachSynapse(bi, bo, bn);
   }
}

backNet::~backNet()
{

}

backNeuron *backNet::createNeuron(neuron *nr)
{
   //fix this -> centralize the overflow check
   if (maxNeurons == 0 || _graph->nodeCount() < maxNeurons)
   {
      backNeuron *bn = new backNeuron(nr);

      attachNeuron(bn);

      return bn;
   }

   return NULL;
}

//backpropagation training class
backProp::backProp(neuralNet *net, const nsignal &neta)
{
   eta = neta;

   _forwardNet = net;
   _forwardNet->initWeights(-1.0f, 1.0f); _forwardNet->initBias(-1.0f, 1.0f);
   
   _backNet = new backNet(_forwardNet);
 

   Real mnum = 1.0f/(Real)net->outputSize();
   _errorFunc = (afunction *) new multiplyFunc(new constFunction(mnum), \
					       new squareFunction());
   _derFunc = _errorFunc->getDerivative();

}

backProp::~backProp()
{

   if (_backNet) delete _backNet;

   if (_errorFunc) delete _errorFunc;
   if (_derFunc) delete _derFunc;
}

void backProp::addData(vector<nsignal> &input, vector<nsignal> &output)
{
   if (input.size() == _forwardNet->inputSize() && output.size() == _forwardNet->outputSize())
   {
      _inData.push_back(input);
      _ouData.push_back(output);
   }
}

void backProp::stepEpoch()
{
   
   for (unsigned i = 0; i < _inData.size(); i++)
   {
      //forward propagate
      _forwardNet->setInput(_inData[i]);
      
      _forwardNet->updateLoop(0);
   
      //set error at backNet input
      for (unsigned j = 0; j < _backNet->inputSize(); j++)
      {
	 backNeuron *bn = (backNeuron *) _backNet->getInputNeuron(j);
	 neuron *nr = (neuron *) bn->getNeuron();
	 
	 //fix this - better in/output indexing
	 nsignal insig = _derFunc->calculate(nr->getOutput() - _ouData[i][j]);
	 
	 bn->setInput(insig);
      }

      //backward propagate
      _backNet->updateLoop(0);

      //read error at synapses and neurons
      accumulateErrors();
   }

   //update weights and bias
   agraph *gp = _backNet->getGraph();
   nsignal coef = eta / (nsignal)_inData.size();

   for (nodeMap::const_iterator it = gp->nodeMapBegin(); it != gp->nodeMapEnd(); ++it)
   {
      backNeuron *bn = (backNeuron *)it->second->getProperty();
      neuron *nr = bn->getNeuron();

      nsignal nw = nr->getWeight() - coef * bn->acc;

      //cout << "N-ACC: " << -coef * bn->acc << endl;

      //bn->setWeight(nw);
      nr->setWeight(nw);

      bn->acc = 0.0f;

   }

   for (edgeMap::const_iterator it = gp->edgeMapBegin(); it != gp->edgeMapEnd(); ++it)
   {
      backSynapse *bs = (backSynapse *)it->second->getProperty();
      synapse *sn = bs->getSynapse();

      nsignal nw = bs->getWeight() - coef * bs->acc;

      //cout << "S-ACC: " << -coef * bs->acc << endl;

      bs->setWeight(nw);
      sn->setWeight(nw);

      bs->acc = 0.0f;

   }  

}

void backProp::trainLoop(const unsigned &maxiter)
{
   for (unsigned i = 0; i < maxiter; i++)
   {
      stepEpoch();
   }
}


void backProp::accumulateErrors()
{
   agraph *gp = _backNet->getGraph();

   for (nodeMap::const_iterator it = gp->nodeMapBegin(); it != gp->nodeMapEnd(); ++it)
   {
      backNeuron *bn = (backNeuron *)it->second->getProperty();
      bn->acc += bn->getOutput();
   }

   //accumulate synapses
   for (edgeMap::const_iterator it = gp->edgeMapBegin(); it != gp->edgeMapEnd(); ++it)
   {
      backSynapse *bs = (backSynapse *)it->second->getProperty();
     
      //fix this
      backNeuron *ibn = (backNeuron *)bs->getEdge()->inNode()->getProperty();
      backNeuron *obn = (backNeuron *)bs->getEdge()->outNode()->getProperty();

      bs->acc += obn->getNeuron()->getOutput() * ibn->getOutput();
   } 
}

Real backProp::netError()
{
   Real err = 0.0f;

   for (unsigned i = 0; i < _inData.size(); i++)
   {
      _forwardNet->setInput(_inData[i]);

      _forwardNet->updateLoop(0);

      vector<nsignal> outp = _forwardNet->getOutput();

      for (unsigned j = 0; j < outp.size(); j++)
      {
	 err += _errorFunc->calculate(outp[j] - _ouData[i][j]);
	 
      }
   }


   err = err/(Real)_inData.size();

   return err;
}
