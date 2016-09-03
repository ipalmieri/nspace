#ifndef __GENETIC_H__
#define __GENETIC_H__

#include <vector>


namespace advtec {
//gene: part of a solution
class gene
{
 public:
  virtual ~gene() {};

  virtual void mutate(const double& rate) = 0;
  virtual gene* createCopy() = 0;

  virtual void apply() = 0;
};

//chromossome: a solution to a problem
class chromossome
{
 public:
  chromossome(const unsigned& size, const unsigned& generation);
  virtual ~chromossome();


  inline unsigned generation()
  {
    return _generation;
  }
  inline unsigned geneCount()
  {
    return _genes.size();
  }

  static chromossome* crossover(std::vector<chromossome*> vec);

  virtual void mutate(const double& rate, const double& genRate);

  void setGene(const unsigned& position, gene* gen);
  gene* getGene(const unsigned& position);

  unsigned id;

 protected:

  unsigned _generation;

  std::vector<gene*> _genes;

};

//ga generic algorithm
class gaMethod
{
 public:
  gaMethod();
  ~gaMethod();

  void stepGeneration();

  inline unsigned populationSize()
  {
    return _population.size();
  }
  chromossome* getSolution(const unsigned& index)
  {
    return _population[index];
  }

  double breedCoef; //first % to breed
  double inertCoef; // first % do not mutate
  double deathRate;
  double popMutRate; //% population that mutates
  double chrMutRate; //% of genes in a chrm that mutates
  double genMutRate; //max abs delta over current a gene current value
  unsigned parentReq;  //parents required to breed
  unsigned breedProd;  //chirldren per breed
  unsigned maxPopulation;

 protected:

  void rank();
  void select();
  void breed();
  void mutate();

  std::vector<chromossome*> _population;

  unsigned _gcount;
  unsigned _nextid;

  virtual void evalPopulation() = 0;
};
}

#endif
