#include "neuralapp/kohonen.h"

#include <stdlib.h>
#include <cmath>
#include "multimedia.h"

#include <iostream>
#include <iomanip>

using namespace neural;
using namespace tools;
using namespace std;

kohonenNet::kohonenNet(const unsigned int& ninputs) : neuralNet()
{
  //there are no meaning for activation function in a hmap
  afunction* func = (afunction*) new idFunction();
  _functions.push_back(func);

  for(unsigned int i = 0; i < ninputs; i++) {
    ineuron* nr = createInput();
    nr->setFunction(func);
  }
}

kohonenNet::~kohonenNet()
{

}


void kohonenNet::addOutput(const double& x, const double& y, const double& z)
{
  //fix this - make generic factory func if possible
  if(maxNeurons == 0 || _graph->nodeCount() < maxNeurons) {
    kneuron* nr = new kneuron();

    attachNeuron(nr);

    //<--- end of section to be fixed

    nr->setPosition(x, y, z);
    _ovars.push_back(nr);

    //sets function
    if(_functions.size() == 1) {
      nr->setFunction(_functions[0]);
    } else {
      afunction* func = (afunction*) new idFunction();
      _functions.push_back(func);
      nr->setFunction(func);
    }


    //connects the neuron to the input
    for(unsigned int w = 0; w < _ivars.size(); w++) {
      neuralNet::connectNeurons(nr, _ivars[w]);
    }

  }
}

//returns the current input BMU
kneuron* kohonenNet::inputBMU()
{
  nsignal mindiff;
  kneuron* min_nr = NULL;
  nsignal winput;

  if(_ovars.size() > 0) {

    //find the BMU
    vector<neuron*>::iterator it = _ovars.begin();
    min_nr = (kneuron*)(*it);
    mindiff = min_nr->calcWInput();
    ++it;

    for(; it != _ovars.end(); ++it) {
      winput = (*it)->calcWInput();
      if(mindiff > winput) {
        mindiff = winput;
        min_nr = (kneuron*)(*it);
      }
    }

  }

  return min_nr;
}

double kohonenNet::avgDist(kneuron* nr, const double& radius)
{
  double dist;
  double sump;
  unsigned nc;

  sump = 0.0;
  nc = 0;

  vector<nweight> win = nr->getInputWeights();

  for(vector<neuron*>::iterator it = _ovars.begin(); it != _ovars.end(); ++it) {
    dist = kdistance(((kneuron*)(*it))->getPosition(), nr->getPosition());

    if(dist <= radius && nr != (kneuron*)(*it)) {
      double wdist = 0.0;

      vector<nweight> wout = ((kneuron*)(*it))->getInputWeights();

      for(unsigned i = 0; i < win.size(); i++) {
        wdist += pow(win[i]-wout[i], 2);
      }
      wdist = sqrt(wdist);

      sump += wdist;

      nc++;
    }

  }

  return sump/(double)nc;
}

//fix this
vectorN<nsignal> kohonenNet::spatialUMatrix(const double& radius)
{
  vectorN<nsignal> ret(_ovars.size());

  for (uint i = 0; i < _ovars.size(); i++) {
    ret[i] = avgDist((kneuron*) _ovars[i], radius);
  }

  return ret;
}

double kohonenNet::quantizationError()
{
  kneuron* vec;

  vec = inputBMU();

  return vec->calcWInput();
}

double kohonenNet::topographicError(const double& radius)
{
  kneuron* kbmu = inputBMU();


  if(_ovars.size() > 0 && kbmu != NULL) {
    kneuron* min_nr=NULL;
    double mindiff=0.0;

    //find the second BMU
    vector<neuron*>::iterator it = _ovars.begin();
    for(; it != _ovars.end(); ++it) {
      if((*it)->getNodeId() != kbmu->getNodeId()) {
        double winput = (*it)->calcWInput();

        if(min_nr == NULL || mindiff > winput) {
          mindiff = winput;
          min_nr = (kneuron*)(*it);
        }
      }
    }

    if(kdistance(min_nr->getPosition(), kbmu->getPosition()) < radius) {
      return 0;
    } else {
      return 1;
    }

  }

  return 1;
}

//============kohonenCube class======================
kohonenCube::kohonenCube(const unsigned& ninputs,
                         const unsigned& x,
                         const unsigned& y,
                         const unsigned& z,
                         const double& width,
                         const double& height,
                         const double& depth) : kohonenNet(ninputs)
{
  uint nx = max(x, (uint)1);
  uint ny = max(y, (uint)1);
  uint nz = max(z, (uint)1);
  double nwidth = max(width, 0.0);
  double nheight = max(height, 0.0);
  double ndepth = max(depth, 0.0);

  kposition tpos;

  for(uint k = 0; k < nz; k++) {
    for(uint j = ny; j > 0; j--) {
      for(uint i = 0; i < nx; i++) {
        tpos.x = (nx == 1)? 0.0 : nwidth * (((double)i / (double)(nx - 1)) - 0.5);
        tpos.y = (ny == 1)? 0.0 : nheight * (((double)(j - 1) / (double)(
                                                ny - 1)) - 0.5);
        tpos.z = (nz == 1)? 0.0 : ndepth * (((double)k / (double)(nz - 1)) - 0.5);

        this->addOutput(tpos.x, tpos.y, tpos.z);
      }
    }
  }

  _nx = nx;
  _ny = ny;
  _nz = nz;

  _width = nwidth;
  _height = nheight;
  _depth = ndepth;

  initWeights(0.0, 1.0);
}

kohonenCube::~kohonenCube()
{

}


kTrainer::kTrainer(kohonenNet* kmap, const double& psigma,
                   const double& plambda)
{
  _kmap = kmap;
  _sigma0 = psigma;
  _lambda = plambda;
  _rlimit = psigma;
  _t = 0.0;

  borderMode = false;

  _timedecay = (afunction*) new expFunction();
  _spacedecay = (afunction*) new expFunction();
}

kTrainer::~kTrainer()
{
  delete _timedecay;
  delete _spacedecay;
}

void kTrainer::fitCurrentInput()
{
  _kmap->updateAll();

  kneuron* nr = _kmap->inputBMU();

  if(nr != NULL) {

    //update all weights using the BMU position
    updateWeights(nr->positionX(), nr->positionY(), nr->positionZ());

    nr->increaseHits();

  }
}


void kTrainer::updateWeights(const double& px, const double& py,
                             const double& pz)
{

  double t = _t;

  kposition mpos;
  mpos.x = px;
  mpos.y = py;
  mpos.z = pz;

  //update all weights based on a given BMU location
  for(vector<neuron*>::iterator it = _kmap->_ovars.begin();
      it != _kmap->_ovars.end(); ++it) {
    double dist = kdistance(mpos, ((kneuron*)(*it))->getPosition());
    gnode* nd = (*it)->getNode();

    //update weights of a neuron
    synapse* sn;
    nweight wt;
    nsignal dt;

    nsignal thetan = theta(dist);
    nsignal alfan = alfa(t);

    for(edgeMap::const_iterator eit = nd->inputEdgesBegin();
        eit != nd->inputEdgesEnd(); ++eit) {
      sn = (synapse*)(eit->second)->getProperty();
      wt = sn->getWeight();
      dt = sn->getNOutput();

      wt = wt + thetan*alfan*(dt - wt);

      sn->setWeight(wt);

    }
  }

  _t = _t + 1.0;
}

//temporal decay function
double kTrainer::alfa(const double& t)
{
  return _timedecay->calculate(-t/_lambda);
}

//spatial decay function
double kTrainer::theta(const double& dist)
{

  if(borderMode)
    if(dist > _rlimit) {
      return 0.0;
    }

  return _spacedecay->calculate(-dist*dist/(2*_sigma0*_sigma0));
}


void kMan::printKmap()
{
  cout << endl << "==== PRINTING KNEURONS FOR NEURAL NET ====" << endl;

  for(vector<neuron*>::iterator it = _kmap->_ovars.begin();
      it != _kmap->_ovars.end(); ++it) {
    cout << "***NEURON " << (*it)->getNodeId();
    cout << " hits: " << ((kneuron*)(*it))->hits();

    //fix this - must be a generic vector position
    kposition npos = ((kneuron*)(*it))->getPosition();
    cout << " [" << setw(5) << setiosflags(ios::fixed);
    cout << setprecision(2) << npos.x;
    cout << " ";
    cout << setprecision(2) << npos.y;
    cout << " ";
    cout << setprecision(2) << npos.z;
    cout << "]";
    //---

    cout << endl;
  }

  cout << "==== END ====" << endl << endl;
}

void kMan::printUMatrix(const double& radius)
{
  cout << endl << "==== PRINTING UMATRIX FOR NEURAL NET ====" << endl;

  for(vector<neuron*>::iterator it = _kmap->_ovars.begin();
      it != _kmap->_ovars.end(); ++it) {
    cout << "NID " << setw(5) << (*it)->getNodeId();
    cout << "     hits: " << setw(5) << ((kneuron*)(*it))->hits();

    cout << "     FUNC: " << setw(5) << setiosflags(ios::fixed);
    cout << setprecision(2) << (*it)->calcWInput();


    cout << "     avgDist: " << setw(5) << setiosflags(ios::fixed);
    cout << setprecision(2) << _kmap->avgDist((kneuron*)(*it), radius);

    cout << "     Weights:";
    vector<synapse*> outp = (*it)->getInputSynapses();
    for(uint i = 0; i < outp.size(); i++) {
      cout << " " << setw(6) << setiosflags(ios::fixed);
      cout << setprecision(2) << outp[i]->getWeight();
    }

    neuron* bm = _kmap->inputBMU();
    if(bm->getNodeId() == (*it)->getNodeId()) {
      cout << "<==";
    }

    cout << endl;
  }

  cout << "==== END ====" << endl << endl;
}

void kMan::saveOutputImage(const string& filename)
{
  saveImage(filename, _kmap->getOutput());
}

void kMan::saveUMatrixImage(const string& filename, const double& radius)
{
  vectorN<nsignal> outp = _kmap->spatialUMatrix(radius);

  nsignal omax, omin = outp[0];
  for(uint i = 1; i < outp.size(); i++) {
    omax = max(omax, outp[i]);
    omin = min(omin, outp[i]);
  }

  for(uint i = 0; i < outp.size(); i++) {
    outp[i] = (outp[i] - omin)/(omax - omin);

    //outp[i] = 1.0 - outp[i];
  }


  saveImage(filename, outp);

}

void kMan::saveImage(const string& filename, vectorN<nsignal> outp)
{
  double width = _kmap->xNeurons();
  double height = _kmap->yNeurons();


  if(outp.size() == width*height) {

    imageMap pic(width, height, 1);


    for(uint i = 0; i < outp.size(); i++) {
      pic[i] = (imageMap::pixel) (outp[i] * 255);
    }

    writeJPEG((char*)filename.c_str(), pic);

  }

}



