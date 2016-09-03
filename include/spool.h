#ifndef __SPOOL_H__
#define __SPOOL_H__

#include <list>
#include "timemark.h"

namespace tools {

template<typename T> struct poolElement {
  T data;
  timeMark mark;
};


////////////////////////////////////////////////////////////////
/// staticPool: keep poolElements for a fixed amount of time
/// flushed when time expires, or when full
//////////////////////////////////////////////////////////////
template<typename T> class staticPool
{
 public:

  typedef std::list<T> poolList;

  staticPool(const uint32_t& maxTTL, const uint32_t& maxSize)
  {
    _maxTTL = maxTTL;
    _maxSize = maxSize;
    _lastUpdate = timeMark::get();
  }

  ~staticPool()
  {
    _elements.clear();
  }

  void addElement(const T& e)
  {
    _elements.push_back(e);
  }

  void appendList(const poolList& l)
  {
    for (auto it = l.begin(); it != l.end(); ++it) {
      addElement(*it);
    }
  }

  void getElements(poolList& l)
  {
    updateAll(l, false);
  }

  void flush(poolList& l)
  {
    updateAll(l, true);
  }

  inline uint32_t size()
  {
    return _elements.size();
  }



 protected:

  void updateAll(poolList& outlist, const bool& flush)
  {
    timeMark tref = timeMark::get();

    bool clearit = ((tref.elapsedSeconds(_lastUpdate) >= _maxTTL) ||
                    flush);


    if (clearit) {
      for (auto it = _elements.begin(); it != _elements.end(); ++it) {
        outlist.push_back(*it);
      }

      _elements.clear();

    }
  }

  uint32_t _maxTTL;      ///< time to keep objects in the pool
  uint32_t _maxSize;     ///< maximum size of the pool
  timeMark _lastUpdate;  ///< last time pool was updated in secs
  poolList _elements;    ///< list of elements inside the pool
};
}

#endif //__SPOOL_H__
