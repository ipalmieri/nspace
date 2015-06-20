#ifndef __NEURALNET_H__
#define __NEURALNET_H__

#include <map>
#include "graph/dgraph.h"
#include "neuron.h"
#include "synapse.h"
#include "ineuron.h"

#define _MIN_DELTA 0.0000001

namespace neural
{
   typedef tools::dgraph<neuron *, synapse *> agraph;


   class neuralNet
   {
     public:
   
      friend class netManager;
      
      virtual neuron *createNeuron();
      virtual void deleteNeuron(neuron *nr);
      virtual void deleteNeuron(const tools::nodeId &id);
      neuron *getNeuron(const tools::nodeId &id);
      synapse *getSynapse(const tools::edgeId &id);

      virtual void connectNeurons(neuron *nr1, neuron *nr2); 
      virtual void disconnectNeurons(neuron *nr1, neuron *nr2);

      inline unsigned int neuronCount() const { return _graph->nodeCount(); }
      inline unsigned int synapseCount() const { return _graph->edgeCount(); }
      
      void updateAll();
      void updateNeurons();
      void updateSynapses();
      bool updateLoop(const unsigned &maxiter, const tools::Real &mindelta=_MIN_DELTA);

      virtual void setInput(const std::vector<nsignal> &insignal);
      inline unsigned int inputSize() const { return _ivars.size(); }
      inline ineuron *getInputNeuron(const unsigned int &index) { return _ivars[index]; }


      std::vector<nsignal> getOutput() const;
      inline unsigned int outputSize() const { return _ovars.size(); }
      inline neuron *getOutputNeuron(const unsigned int &index) { return _ovars[index]; }

      void initWeights(const nsignal &min, const nsignal &max);
      void initBias(const nsignal &min, const nsignal &max);

      unsigned int maxNeurons;
      unsigned int maxSynapses;
      
      agraph *getGraph() { return _graph; }

     protected:
      
      neuralNet();
      virtual ~neuralNet(); 

      ineuron *createInput();
      void attachNeuron(neuron *nr);
      void attachSynapse(neuron *nr1, neuron *nr2, synapse *sn);

      agraph *_graph;
      std::vector<ineuron *> _ivars;
      std::vector<neuron *> _ovars;
      std::vector<afunction *> _functions;

     private: 

   };
}

#endif
