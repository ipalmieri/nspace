#ifndef __TIMEMARK_H__
#define __TIMEMARK_H__

#include <stdint.h>

namespace tools {
//////////////////////////////////////////////
/// Steady clock mark in time for measurement
//////////////////////////////////////////////
class timeMark
{

 public:

  //timeMark() { (*this) = get(); }
  timeMark() {}
  ~timeMark() {}

  static timeMark get();

  double elapsedSeconds(const timeMark& mk);

  bool operator<(const timeMark& tm) const;
  bool operator>(const timeMark& tm) const;
  bool operator==(const timeMark& tm) const;
  bool operator!=(const timeMark& tm) const;
  bool operator<=(const timeMark& tm) const;
  bool operator>=(const timeMark& tm) const;


 protected:

  //timeMark() {}
  timeMark(const uint64_t& secs, const uint64_t& nans) : _secs(secs),
    _nans(nans) {}

  uint64_t _secs; ///< seconds since epoch
  uint64_t _nans; ///< nanoseconds on top of the seconds
};
}

#endif //__TIMEMARK_H__
