#include <stdlib.h>
#include <assert.h>
#include "neuralnet.h"

using namespace tools;
using namespace neural;
using namespace std;

neuralNet::neuralNet()
{
   _graph = new agraph();
   maxNeurons = 0;
   maxSynapses = 0;
}

neuralNet::~neuralNet()
{
   //must delete all neurons and synapses before 

   for (nodeMap::const_iterator it = _graph->nodeMapBegin(); it != _graph->nodeMapEnd(); ++it)
   {
      neuron *nr = (neuron *) it->second->getProperty();
      
      delete nr;
   }

   for (edgeMap::const_iterator it = _graph->edgeMapBegin(); it != _graph->edgeMapEnd(); ++it)
   {
      synapse *sn = (synapse *) it->second->getProperty();
      
      delete sn;
   }


   // now delete all functions
   for (vector<afunction *>::iterator it = _functions.begin(); it != _functions.end(); ++it)
   {
      delete (*it);
   }
   

   //delete the graph
   delete _graph;
}

neuron *neuralNet::createNeuron()
{
   if (maxNeurons == 0 || _graph->nodeCount() < maxNeurons)
   {
      neuron *nr = new neuron();
      
      attachNeuron(nr);

      return nr;
   }

   return NULL;
}

void neuralNet::deleteNeuron(neuron *nr)
{
   if (nr != NULL)
   {
      _graph->deleteNode(nr->getNodeId());

      delete nr;
   }
}

void neuralNet::deleteNeuron(const nodeId &id)
{
   gnode *n = _graph->getNode(id);
  
   if (n != NULL)
   {
      neuron *nr = (neuron *) n->getProperty();
      
      _graph->deleteNode(n);
  
      delete nr;
   }
}

neuron *neuralNet::getNeuron(const nodeId &id)
{
   gnode *n = _graph->getNode(id);

   if (n != NULL)
   {
      return (neuron *) n->getProperty();
   }

   return NULL;
}

synapse *neuralNet::getSynapse(const edgeId &id)
{
   gedge *e = _graph->getEdge(id);

   if (e != NULL)
   {
      return (synapse *) e->getProperty();
   }

   return NULL;
}


void neuralNet::connectNeurons(neuron *nr1, neuron *nr2)
{
   if (maxSynapses == 0 || _graph->edgeCount() + 1 <= maxSynapses)
   {
      if (nr1 != NULL && nr2 != NULL)
      { 
	 synapse *sn = new synapse();	 

	 attachSynapse(nr1, nr2, sn);
      }
   }
}

void neuralNet::disconnectNeurons(neuron *nr1, neuron *nr2)
{

   if (nr1 != NULL && nr2 != NULL)
   { 
      gnode *n1, *n2;
      
      n1 = _graph->getNode(nr1->getNodeId());
      n2 = _graph->getNode(nr2->getNodeId());

      gedge *ed = _graph->getEdge(n1, n2);

      if (ed != NULL)
      {
	 synapse *sn = (synapse *) ed->getProperty();

	 if (sn != NULL) delete sn;
      }

      _graph->disconnectNodes(n1, n2);
      
   }

   //verificar se algum neuron ficou sem output 
   //e coloca-lo na lista para delete
}

void neuralNet::updateAll()
{
   updateNeurons();
   updateSynapses();
}

void neuralNet::updateNeurons()
{
   neuron *nr;

   for (nodeMap::const_iterator it = _graph->nodeMapBegin(); it != _graph->nodeMapEnd(); ++it)
   {
      nr = (neuron *) it->second->getProperty();
      
      nr->update();
   }
}

void neuralNet::updateSynapses()
{
   synapse *sn;

   for (edgeMap::const_iterator it = _graph->edgeMapBegin(); it != _graph->edgeMapEnd(); ++it)
   {
      sn = (synapse *) it->second->getProperty();
      
      sn->update();
   }
}

bool neuralNet::updateLoop(const unsigned &maxiter, const Real &mindelta)
{
   unsigned iter = 0;
   bool convStatus;
   Real oldval;

   do {
      convStatus = true;

      //update neurons
      for (nodeMap::const_iterator it = _graph->nodeMapBegin(); it != _graph->nodeMapEnd(); ++it)
      {
	 neuron *nr = (neuron *) it->second->getProperty();
	 
	 oldval = nr->getOutput();
	 nr->update();

	 if (abs(oldval - nr->getOutput()) >= mindelta) convStatus = false;
      }

      //update synapses
      for (edgeMap::const_iterator it = _graph->edgeMapBegin(); it != _graph->edgeMapEnd(); ++it)
      {
	 synapse *sn = (synapse *) it->second->getProperty();
	 
	 oldval = sn->getOutput();
	 sn->update();
	 
	 if (abs(oldval - sn->getOutput()) >= mindelta) convStatus = false;
      }
 
   } while ((iter++ < maxiter || maxiter == 0) && !convStatus);

 
   if (convStatus)
      return true;

   return false;
}

void neuralNet::setInput(const vector<nsignal> &insignal)
{
   if (insignal.size() == _ivars.size())
   {
      for (unsigned int i = 0; i < _ivars.size(); i++)
      {
	 _ivars[i]->setInput(insignal[i]);
      }
   }
}

vector<nsignal> neuralNet::getOutput() const
{
   vector<nsignal> ret;
   
   for (unsigned int i = 0; i < _ovars.size(); i++)
      ret.push_back(_ovars[i]->getOutput());
 
   return ret;
}

ineuron *neuralNet::createInput()
{
   ineuron *nr = new ineuron();

   attachNeuron(nr);

   _ivars.push_back(nr);

   return nr;
}


void neuralNet::initWeights(const nsignal &min, const nsignal &max)
{
   srand((unsigned) time(NULL));

   for (edgeMap::const_iterator it = _graph->edgeMapBegin(); it != _graph->edgeMapEnd(); ++it)
   {
      synapse *sn = (synapse *) (it->second)->getProperty();

      sn->setWeight(min + (max - min)*((nweight) rand()/RAND_MAX));
   }

}

void neuralNet::initBias(const nsignal &min, const nsignal &max)
{
   srand((unsigned) time(NULL));

   for (nodeMap::const_iterator it = _graph->nodeMapBegin(); it != _graph->nodeMapEnd(); ++it)
   {
      neuron *nr = (neuron *) (it->second)->getProperty();

      nr->setWeight(min + (max - min)*((nweight) rand()/RAND_MAX));
   }

}

void neuralNet::attachNeuron(neuron *nr)
{
   gnode *nd_nr = _graph->createNode(nr); 

   if (nd_nr) nr->attachNode(nd_nr);
}

void neuralNet::attachSynapse(neuron *nr1, neuron *nr2, synapse *sn)
{
   gnode *n1 = nr1->getNode();
   gnode *n2 = nr2->getNode();

   gedge *ed = _graph->connectNodes(n1, n2, sn);

   sn->attachEdge(ed);
}
