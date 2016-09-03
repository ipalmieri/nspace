#include <ctime>
#include "timemark.h"


using namespace tools;

timeMark timeMark::get()
{
  timespec tspec;

  clock_gettime(CLOCK_MONOTONIC, &tspec);

  return timeMark(tspec.tv_sec, tspec.tv_nsec);
}

double timeMark::elapsedSeconds(const timeMark& tm)
{
  return ((_secs - tm._secs)*10e9 + _nans - tm._nans) / 10e9;
}


bool timeMark::operator<(const timeMark& tm) const
{
  if (_secs < tm._secs) {
    return true;
  } else if (_secs == tm._secs) {
    return (_nans < tm._nans);
  }

  return false;
}

bool timeMark::operator>(const timeMark& tm) const
{
  return !((*this) < tm || (*this) == tm);
}

bool timeMark::operator==(const timeMark& tm) const
{
  return (_secs == tm._secs &&
          _nans == tm._nans);
}

bool timeMark::operator!=(const timeMark& tm) const
{
  return !((*this) == tm);
}

bool timeMark::operator<=(const timeMark& tm) const
{
  return (*this) == tm || (*this) < tm;
}

bool timeMark::operator>=(const timeMark& tm) const
{
  return (*this) == tm || (*this) > tm;
}

