#ifndef __NEURON_H__
#define __NEURON_H__

#include <string>
#include <vector>
#include <map>
#include "graph/dnode.h"
#include "afunction.h"

namespace neural
{
   class synapse;
   class neuron;

   typedef unsigned short groupId;
   typedef tools::dnode<neuron *, synapse *> gnode;   
   typedef gnode::dnodeMap nodeMap;

   class neuron 
   {
     public:
  
      friend class neuralNet;
     
      inline tools::nodeId getNodeId() { return _node->id(); }
      inline gnode *getNode() { return _node; }
 
      virtual void update();

      virtual nsignal calcWInput();
      inline nsignal getOutput() { return _output; }

    
      inline void setGroup(const groupId &gid) { _gid = gid; }
      inline groupId getGroup() { return _gid; }

      inline afunction *getFunction() { return _actfunc; }
      inline void setFunction(afunction *func) { _actfunc = func; }     
 
      inline nweight getWeight() { return _w0bias; }
      inline void setWeight(const nweight &w) { _w0bias = w; }

      std::vector<synapse *> getInputSynapses() const;
      std::vector<synapse *> getOutputSynapses() const;

     protected:
    
      neuron(const groupId &gid = 0);
      virtual ~neuron();     

      inline void setOutput(const nsignal &value) { _output = value; }
      inline void attachNode(gnode *node) { _node = node; }
      virtual void normalizeWeights();

      gnode *_node;
      groupId _gid; //fix this - use group list
      nsignal _output;
      afunction *_actfunc;
      nweight _w0bias;
     
     private:


   };

}



#endif
