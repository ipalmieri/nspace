#ifndef __TIMESERIE_H__
#define __TIMESERIE_H__

#include <map>
#include <date.h>

namespace tools {
//////////////////////////////////////////////
/// List of any object associated with dates
//////////////////////////////////////////////
template <typename T> class timeserie
{
 public:
  timeserie() {}
  ~timeserie() {}

  void addEntry(const date& dt, const T& value);
  T getValue(const date& dt);

  inline T& operator[](const date& dt)
  {
    return _serie[dt];
  }
  inline unsigned int length()
  {
    return (unsigned int) _serie.size();
  }

  inline typename std::map<date, T>::iterator begin()
  {
    return _serie.begin();
  }
  inline typename std::map<date, T>::iterator end()
  {
    return _serie.end();
  }


 protected:

  std::map<date, T> _serie;
};
}

#endif //__TIMESERIE_H__
