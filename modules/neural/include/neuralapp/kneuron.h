#ifndef __KNEURON_H__
#define __KNEURON_H__

#include <neuron.h>
#include <synapse.h>

namespace neural {

//fix this - position must be of generic dimension Rn vs R3
typedef struct {
  double x;
  double y;
  double z;
} kposition;

inline double kdistance(const kposition& p1, const kposition& p2)
{
  return sqrt( (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y) +
               (p1.z - p2.z)*(p1.z - p2.z));
}


class kneuron : public neuron
{
 public:

  friend class kohonenNet;
  nsignal calcWInput();

  inline double positionX() const
  {
    return _pos.x;
  }
  inline double positionY() const
  {
    return _pos.y;
  }
  inline double positionZ() const
  {
    return _pos.z;
  }
  inline kposition getPosition() const
  {
    return _pos;
  }


  inline unsigned hits() const
  {
    return _hits;
  }
  inline void increaseHits()
  {
    _hits++;
  };

  std::vector<nweight> getInputWeights() const;

  std::string tag;

 protected:

  kposition _pos;
  unsigned _hits;

  kneuron(const groupId& gid = 0);
  ~kneuron();

  inline void setPosition(const double& x,
                          const double& y,
                          const double& z=0.0)
  {
    _pos.x = x;
    _pos.y = y;
    _pos.z = z;
  }

};
}
#endif
