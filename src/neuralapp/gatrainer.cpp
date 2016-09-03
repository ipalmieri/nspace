#include "neuralapp/gatrainer.h"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include "sorting.h"

using namespace std;
using namespace tools;
using namespace advtec;
using namespace neural;

//==================== gaWeight ===========================
void gaWeight::mutate(const double& rate)
{
  double nrate = rate;//min(max(rate, (double)1.0f), (double)0.0f);

  nrate = nrate * (2.0f * (double)rand()/RAND_MAX - 1.0f);

  value += nrate;

}

gene* gaWeight::createCopy()
{
  gaWeight* ret = new gaWeight(_synapse);

  ret->value = value;

  return ret;
}

void gaWeight::initRand(const double& start, const double& end)
{
  value = start + (end - start) * (double)rand()/RAND_MAX;
}


//==================== gaBias ===========================
void gaBias::mutate(const double& rate)
{
  double nrate = rate;//min(max(rate, (double)1.0f), (double)0.0f);

  nrate = nrate * (2.0f * (double)rand()/RAND_MAX - 1.0f);

  value += nrate;

}

gene* gaBias::createCopy()
{
  gaBias* ret = new gaBias(_neuron);

  ret->value = value;

  return ret;
}

void gaBias::initRand(const double& start, const double& end)
{
  value = start + (end - start) * (double)rand()/RAND_MAX;
}

//=================== gaNetTrainer ========================
gaNetTrainer::gaNetTrainer(neuralNet* net, const unsigned& pinit)
  : gaMethod()
{
  _net = net;

  init(pinit);
}

gaNetTrainer::~gaNetTrainer()
{

}

void gaNetTrainer::addData(vectorN<nsignal>& input, vectorN<nsignal>& output)
{
  assert(input.size() == _net->inputSize() &&
         output.size() == _net->outputSize());

  if (input.size() == _net->inputSize() && output.size() == _net->outputSize()) {
    _inData.push_back(input);
    _ouData.push_back(output);
  }
}

void gaNetTrainer::trainLoop(const unsigned& maxgenerations)
{
  while (_gcount < maxgenerations) {
    stepGeneration();
  }

  setSolution(_population[0]);
}

//creates pinit chromossomes and bound genes to start evolution
void gaNetTrainer::init(const unsigned& pinit)
{
  unsigned scount = _net->synapseCount();
  unsigned ncount = _net->neuronCount();

  for (unsigned i = 0; i < pinit; i++) {
    chromossome* cron = new chromossome(scount + ncount, 0);

    agraph* gr = _net->getGraph();

    unsigned index = 0;

    for (edgeMap::const_iterator it = gr->edgeMapBegin(); it != gr->edgeMapEnd();
         ++it) {
      synapse* sn = (synapse*)it->second->getProperty();
      gaWeight* gn = new gaWeight(sn);

      //fix this
      gn->initRand(-1.0f, 1.0f);

      cron->setGene(index, gn);

      index++;
    }

    for (nodeMap::const_iterator it = gr->nodeMapBegin(); it != gr->nodeMapEnd();
         ++it) {
      neuron* nr = (neuron*)it->second->getProperty();

      //fix this
      gaBias* gb = new gaBias(nr);
      gb->initRand(-1.0f, 1.0f);
      cron->setGene(index, gb);

      index++;
    }

    cron->id = _nextid++;
    _population.push_back(cron);
  }
}

void gaNetTrainer::evalPopulation()
{
  vector<chromEval> solutions;

  //evaluate all available chromossomes
  for (unsigned i = 0; i < _population.size(); i++) {
    chromEval sol;

    sol.error = evalError(_population[i]);
    sol.solution = _population[i];

    solutions.push_back(sol);
  }

  //now rank them
  quicksort(solutions);

  //================TEST LOOP
  /*for (unsigned i = 0; i < solutions.size(); i++)
  {
     chromossome *chr = solutions[i].solution;
     cout << "S: " << setw(5) << chr->id << " ( ";

     for (unsigned j = 0; j < chr->geneCount(); j++)
     {
  cout << setw(6) << setiosflags(ios::fixed) << setprecision(3);
  cout << ((gaWeight *)chr->getGene(j))->value << " ";
     }

     cout << ")  E: ";

     cout << solutions[i].error << endl;
     }*/
  //===============TEST LOOP

  for (unsigned i = 0; i < solutions.size(); i++) {
    _population[i] = solutions[i].solution;
  }

}

//returns the accumulated error for some solution
nsignal gaNetTrainer::evalError(chromossome* chr)
{
  nsignal erro = 0.0f;
  unsigned mc = 0;

  setSolution(chr);

  //fix this - assert _inData.size() > 0

  for (unsigned i = 0; i < _inData.size(); i++) {
    _net->setInput(_inData[i]);

    //fix this
    bool tconv = _net->updateLoop(0);

    if (tconv) {
      //erro = max((double)erro, (double)realAbs(outputError(_ouData[i])));
      erro += (double) std::abs(outputError(_ouData[i]));
      mc++;
    }/*
      else
	 cout << "ERRO!! ";

      cout << "EvalError(): S:" << chr->id << "    INPUT ( ";
      for (unsigned j = 0; j < _inData[i].size(); j++)
      {
	 cout << _inData[i][j] << " ";
      }

      cout << ")     DESIRED (";
      for (unsigned j = 0; j < _ouData[i].size(); j++)
      {
	 cout << _ouData[i][j] << " ";
      }

      cout << ")     OUTPUT (";
      vector<nsignal> ot = _net->getOutput();
      for (unsigned j = 0; j < ot.size(); j++)
      {
	 cout << ot[j] << " ";
      }
      cout << ")";
      cout << endl;
      */

  }

  return erro/(double)mc;
}

//loads a chromossome in the network
void gaNetTrainer::setSolution(chromossome* chr)
{
  for (unsigned i = 0; i < chr->geneCount(); i++) {
    gene* gw = chr->getGene(i);

    if (gw != NULL) {
      gw->apply();
    }
  }
}

//returns the error for current output vs desired (ynet)
Real gaNetTrainer::outputError(vectorN<nsignal>& ynet)
{
  Real err = 0.0f;
  vectorN<nsignal> outp = _net->getOutput();

  for (unsigned i = 0; i < outp.size(); i++) {
    err += (ynet[i] - outp[i])*(ynet[i] - outp[i]);
  }

  return err/(Real)outp.size();
}

chromossome* gaNetTrainer::getSolution(const unsigned& index)
{
  if (index < _population.size()) {
    return _population[index];
  }

  return NULL;
}


void gaNetTrainer::printSolutions()
{
  for (unsigned i = 0; i < _population.size(); i++) {
    chromossome* ch = _population[i];

    cout << "S";
    cout << setw(5);
    cout << ch->id << ": (   ";

    for (unsigned j = 0; j < ch->geneCount(); j++) {
      gaWeight* gw = (gaWeight*) ch->getGene(j);

      cout << gw->getSynapse()->getEdgeId() << ":";
      cout << setw(6) << setiosflags(ios::fixed) << setprecision(3);
      cout << gw->value << "   ";
    }

    cout << ")";

    /*
    cout << " E: ";

    cout << setw(6) << setiosflags(ios::fixed) << setprecision(3);
    cout << evalError(_population[i]);
    */

    cout << endl;
  }
}

