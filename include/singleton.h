#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <stdio.h>

namespace tools {

///////////////////////////////////////
/// Simple single threaded singleton
///////////////////////////////////////
template<class T> class singleton
{
 public:

  static T* getInstance()
  {
    if (!_instance) {
      _instance = new T;
      std::atexit(stopInstance);
    }
    return _instance;
  }

  static void stopInstance()
  {
    if (_instance) {
      delete _instance;
      _instance = NULL;
    }
  }


 protected:

  singleton() {}
  virtual ~singleton()
  {
    stopInstance();
  };

  singleton(singleton const&) {}
  singleton& operator=(singleton const&) {}

  static T* _instance;

};



template<class T> T* singleton<T>::_instance = NULL;
}

#endif //__SINGLETON_H__
