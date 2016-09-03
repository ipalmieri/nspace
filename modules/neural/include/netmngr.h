#ifndef __NETMNGR_H__
#define __NETMNGR_H__

#include <string>
#include "neuralnet.h"
#include "neuralapp/kohonen.h"

namespace neural {

class netManager
{
 public:
  netManager(neuralNet* nrnet);
  ~netManager();

  void saveFile(const std::string& filename);
  void loadFile(const std::string& filename);

  void printNeurons();
  void printSynapses();

 protected:
  neuralNet* _neuralnet;

 private:

};


}


#endif
