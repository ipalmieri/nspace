#include "genetic.h"

#include <stdlib.h>
#include <vector>
#include <algorithm>
#include "aux.h"

using namespace std;
using namespace tools;
using namespace advtec;

chromossome::chromossome(const unsigned& size, const unsigned& generation)
{
  _generation = generation;

  for (unsigned i=0; i < size; i++) {
    _genes.push_back(NULL);
  }

  id = 0;
}

chromossome::~chromossome()
{
  for (unsigned i = 0; i < _genes.size(); i++) {
    delete _genes[i];
  }
}

void chromossome::mutate(const double& rate, const double& genRate)
{
  unsigned mcount = cint((unsigned) ((double) _genes.size() * min((double) 1.0f,
                                     max((double)0.0f, rate))));
  vector<unsigned> chvec;
  unsigned pos;

  for (unsigned i = 0; i < mcount; i++) {
    do {

      pos = randU(0, _genes.size() - 1);

    } while (find(chvec.begin(), chvec.end(), pos) != chvec.end());

    chvec.push_back(pos);

    if (_genes[pos] != NULL) {
      _genes[pos]->mutate(genRate);
    }

  }
}

chromossome* chromossome::crossover(vector<chromossome*> vec)
{
  unsigned pcount = vec.size();
  unsigned csize, gener;

  //if (pcount <= 0) return error

  //calculate size and generation of the child
  csize = vec[0]->geneCount();
  gener = vec[0]->generation();
  for (unsigned i = 1; i < pcount; i++) {
    if (vec[i]->geneCount() > csize) {
      csize = vec[i]->geneCount();
    }

    if (vec[i]->generation() > gener) {
      gener = vec[i]->generation();
    }
  }

  //create the child
  chromossome* child = new chromossome(csize, gener + 1);

  for (unsigned i = 0; i < csize; i++) {
    unsigned sgen;

    do {

      sgen = randU(0, pcount - 1);

    } while (i >= vec[sgen]->geneCount());

    gene* ng = NULL;
    gene* pg = vec[sgen]->getGene(i);

    if (pg != NULL) {
      ng = pg->createCopy();
    }

    child->setGene(i, ng);

  }

  return child;
}

void chromossome::setGene(const unsigned& position, gene* gen)
{
  if (position < _genes.size()) {
    delete _genes[position];

    _genes[position] = gen;
  }
}

gene* chromossome::getGene(const unsigned& position)
{
  if (position < _genes.size()) {

    return _genes[position];
  }

  return NULL;
}


//============= gamethod ====================
gaMethod::gaMethod()
{
  _gcount = 0;

  breedCoef = 1.0f;
  inertCoef = 0.1f;
  deathRate = 0.0f;
  popMutRate = 0.0f;
  chrMutRate = 0.0f;
  genMutRate = 0.0f;
  maxPopulation = 0;

  parentReq = 2;
  breedProd = 1;

  _nextid = 0;
}

gaMethod::~gaMethod()
{
  for (unsigned i = 0; i < _population.size(); i++) {
    delete _population[i];
  }
}

void gaMethod::stepGeneration()
{
  breed();

  mutate();

  rank();

  select();

  _gcount++;
}

//evaluate and order population by fitness
void gaMethod::rank()
{
  evalPopulation();
}

//kill pop*deathRate least adapted chromossomes
void gaMethod::select()
{
  unsigned rlen = cint(max((double)0.0f, min(deathRate,
                           (double) 1.0f))*((double)_population.size()));

  if (maxPopulation != 0 && _population.size() > maxPopulation) {
    rlen = max(rlen, (unsigned) _population.size() - maxPopulation);
  }

  for (unsigned i = 0; i < rlen; i++) {
    delete _population.back();

    _population.pop_back();
  }
}

//breed in proportion parentReq:breedProd
void gaMethod::breed()
{
  unsigned pcount = cint(breedCoef * (double)_population.size()/
                         (double)parentReq);
  vector<chromossome*> children;

  for (unsigned i = 0; i < pcount; i++) {
    vector<chromossome*> parents;

    for (unsigned j = 0; j < parentReq; j++) {
      unsigned ppos;

      do {

        ppos = randU(0, breedCoef * _population.size() - 1);

      } while (find(parents.begin(), parents.end(),
                    _population[ppos]) != parents.end());

      parents.push_back(_population[ppos]);
    }

    for (unsigned j = 0; j < breedProd; j++) {
      chromossome* chr = chromossome::crossover(parents);
      chr->id = _nextid;
      _nextid++;
      children.push_back(chr);
    }

  }

  for (unsigned i = 0; i < children.size(); i++) {
    _population.push_back(children[i]);
  }

}

//mutate pop*mutRate chromossomes
void gaMethod::mutate()
{
  unsigned mcount = cint((unsigned) ((double) _population.size() * popMutRate));
  vector<unsigned> chvec;

  mcount = min(mcount,(unsigned) cint((1.0f - inertCoef)*(double)
                                      _population.size()));
  for (unsigned i = 0; i < mcount; i++) {
    unsigned chind;

    do {

      chind = randU(inertCoef*_population.size(), _population.size() - 1);

    } while (find(chvec.begin(), chvec.end(), chind) != chvec.end());

    chvec.push_back(chind);

    _population[chind]->mutate(chrMutRate, genMutRate);

  }

}
