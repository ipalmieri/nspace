#include "netmngr.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include "neuralnet.h"

using namespace tools;
using namespace neural;
using namespace std;

netManager::netManager(neuralNet* nrnet)
{
  _neuralnet = nrnet;
}

netManager::~netManager()
{

}

void netManager::saveFile(const string& filename)
{
  //fix this TODO
}

void netManager::loadFile(const string& filename)
{
  //fix this TODO
}

//debug functions
void netManager::printNeurons()
{
  cout << endl << "==== PRINTING NEURONS FOR NEURAL NET ====" << endl;

  for (nodeMap::const_iterator it = _neuralnet->_graph->nodeMapBegin();
       it != _neuralnet->_graph->nodeMapEnd(); ++it) {
    gnode* nd = it->second;
    neuron* nr = (neuron*) nd->getProperty();


    cout << "***NEURON " << nd->id();

    cout << "   Output: ";
    cout << setw(5) << setiosflags(ios::fixed) << setprecision(3);
    cout << nr->getOutput() << "   Bias: ";
    cout << setw(5) << setiosflags(ios::fixed) << setprecision(3);
    cout << nr->getWeight() << endl;

    cout << "   I: ";

    for (edgeMap::const_iterator jt = nd->inputEdgesBegin();
         jt != nd->inputEdgesEnd(); ++jt) {
      cout << (jt->second)->inNode()->id() << " ";
    }

    cout <<  endl;

    cout << "   O: ";

    for (edgeMap::const_iterator jt = nd->outputEdgesBegin();
         jt != nd->outputEdgesEnd(); ++jt) {
      cout << (jt->second)->outNode()->id() << " ";
    }

    cout << endl;
  }

  cout << "==== END ====" << endl << endl;
}

void netManager::printSynapses()
{
  cout << endl << "==== PRINTING SYNAPSES FOR NEURAL NET ====" << endl;

  agraph* gr = _neuralnet->_graph;

  for (edgeMap::const_iterator it = gr->edgeMapBegin(); it != gr->edgeMapEnd();
       ++it) {
    gedge* ed = it->second;
    synapse* sn = (synapse*) ed->getProperty();

    cout << "***SYNAPSE " << ed->id();
    cout << ": " << ed->inNode()->id();
    cout << " -> " << ed->outNode()->id();
    cout << " W=" << sn->getWeight();
    cout << endl;
  }

  cout << "==== END ====" << endl << endl;
}



