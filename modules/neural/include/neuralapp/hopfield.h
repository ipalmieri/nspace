#ifndef __HOPFIELD_H__
#define __HOPFIELD_H__

#include "neuralnet.h"

namespace neural
{
   class hneuron : public ineuron
   {
     public:
      friend class hopfieldNet;
      
     protected:
      
     hneuron(const groupId &gid = 0) : ineuron(gid) { _inweight = 0.0; }
      ~hneuron() {}

      void setInput(const nsignal &in) { _input = in; _output = in; }
      
     private:

   };
   

   class hopfieldNet : public neuralNet
   {
     public:
      hopfieldNet(const unsigned int &ncount);
      ~hopfieldNet();

      neuron *createNeuron() { return (neuron *) NULL; }
      void deleteNeuron(neuron *nr) {}
      void deleteNeuron(const tools::nodeId &id) {}

      void connectNeurons(neuron *nr1, neuron *nr2) {}
      void disconnectNeurons(neuron *nr1, neuron *nr2) {}

      void setInput(const std::vector<nsignal> &insignal);
      void addImage();
      
      tools::Real energy();

     protected:

     private:

   };
}

#endif
