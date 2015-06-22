#ifndef __KOHONEN_H__
#define __KOHONEN_H__

#include "neuralnet.h"
#include "kneuron.h"

namespace neural
{
   class kohonenNet:public neuralNet
   {
      friend class kTrainer;
 
     public:
      kohonenNet(const unsigned int &ninputs);
      virtual ~kohonenNet();

      neuron *createNeuron() { return (neuron *) NULL; }
      void deleteNeuron(neuron *nr) {}
      void deleteNeuron(const tools::nodeId &id) {}

      void connectNeurons(neuron *nr1, neuron *nr2) {}
      void disconnectNeurons(neuron *nr1, neuron *nr2) {}

      void addOutput(const double &x, const double &y, const double &z = 0.0);
      kneuron *inputBMU();
 
      double avgDist(kneuron *nr, const double &radius);
      std::vector<nsignal> spatialUMatrix(const double &radius);

      double quantizationError();
      double topographicError(const double &radius);

     protected:

      
     private:

   };

   //3D square kohonenNet
   class kohonenCube : public kohonenNet 
   {
     public:
      friend class kMan;

      kohonenCube(const unsigned &ninputs,
		  const unsigned &x, 
		  const unsigned &y,
		  const unsigned &z,
		  const double &width,
		  const double &height,
		  const double &depth);
      ~kohonenCube();

      inline unsigned xNeurons() const { return _nx; }
      inline unsigned yNeurons() const { return _ny; }      
      inline unsigned zNeurons() const { return _nz; }

      inline double width() const { return _width; }
      inline double height() const { return _height; }
      inline double depth() const { return _depth; }
      
     protected:
      
      unsigned _nx;
      unsigned _ny;
      unsigned _nz;

      double _width;
      double _height;
      double _depth;

   };


   //trainer for kohonenNets
   class kTrainer
   {
     public:
      kTrainer(kohonenNet *kmap, const double &psigma, const double &plambda);
      ~kTrainer();

      void fitCurrentInput();
 
      bool borderMode;

     protected:

      kohonenNet *_kmap;

      double _rlimit;
      double _sigma0;
      double _lambda;
      double _t;

      void updateWeights(const double &px, const double &py, const double &pz);

      double alfa(const double &t);
      double theta(const double &dist);

      afunction *_spacedecay;
      afunction *_timedecay;

   };


   //auxiliary class for 2D and 3D square maps
   class kMan
   {
     public:
      kMan(kohonenCube *kmap) { _kmap = kmap; }
      ~kMan() {}

      void printKmap();
      void printUMatrix(const double &radius);

      void saveOutputImage(const std::string &filename);
      void saveUMatrixImage(const std::string &filename, const double &radius);
      void saveImage(const std::string &filename, std::vector<nsignal> outp);      

     protected:

      kohonenCube *_kmap;
   };
   

   //auxiliary functions
   inline kohonenCube *kreateCube(const unsigned &ninputs,
				  const unsigned &x, 
				  const unsigned &y,
				  const unsigned &z,
				  const double &width,
				  const double &height,
				  const double &depth)
   {
      return new kohonenCube(ninputs, x, y, z, width, height, depth);
   }

      
   inline kohonenCube *kreateMap(const unsigned &ninputs,
				 const unsigned &x, 
				 const unsigned &y,
				 const double &width,
				 const double &height)
   {
      return kreateCube(ninputs, x, y, 1, width, height, 0.0);
   }


}

#endif
