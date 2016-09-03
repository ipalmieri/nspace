#include <sstream>
#include <iomanip>
#include <ctime>
#include "timepoint.h"


using namespace std;
using namespace tools;


timePoint::timePoint(const tm& t, const uint64_t& nano)
{
  _year = t.tm_year + 1900;
  _month = t.tm_mon + 1;
  _day = t.tm_mday;

  _hour = t.tm_hour;
  _minute = t.tm_min;
  _second = t.tm_sec;

  _nano = nano;

  _isDST = (t.tm_isdst > 0);
}


timePoint timePoint::now()
{
  timespec tspec;
  tm t;
  long nano;

  clock_gettime(CLOCK_REALTIME, &tspec);

  localtime_r(&tspec.tv_sec, &t); //well, this seems a good idea

  nano = tspec.tv_nsec;

  return timePoint(t, nano);
}

timePoint timePoint::nowGMT()
{
  timespec tspec;
  tm t;
  long nano;

  clock_gettime(CLOCK_REALTIME, &tspec);

  gmtime_r(&tspec.tv_sec, &t);

  nano = tspec.tv_nsec;

  return timePoint(t, nano);
}

timePoint timePoint::toGMT()
{
  tm t1, t2;

  t1 = structTM();

  time_t tm = mktime(&t1);

  gmtime_r(&tm, &t2);

  return timePoint(t2, _nano);
}

string timePoint::toStr() const
{
  ostringstream output;

  output.fill('0');
  output << setw(2) << _day;
  output << "/";
  output << setw(2) << _month;
  output << "/";
  output << setw(4) << _year;

  output << " ";

  output << setw(2) << _hour;
  output << ":";
  output << setw(2) << _minute;
  output << ":";
  output << setw(2) << _second;
  output << " ";
  output << setw(9) << _nano;

  return output.str();
}

tm timePoint::structTM() const
{
  tm rtm;

  rtm.tm_year = _year - 1900;
  rtm.tm_mon = _month - 1;
  rtm.tm_mday = _day;

  rtm.tm_hour = _hour;
  rtm.tm_min = _minute;
  rtm.tm_sec = _second;


  return rtm;
}

bool timePoint::operator<(const timePoint& tp) const
{
  if (_year < tp._year) {
    return true;
  } else if (_year == tp._year) {
    if (_month < tp._month) {
      return true;
    } else if (_month == tp._month) {
      if (_day < tp._day) {
        return true;
      } else if (_day == tp._day) {
        if (_hour < tp._hour) {
          return true;
        } else if (_hour == tp._hour) {
          if (_minute < tp._minute) {
            return true;
          } else if (_minute == tp._minute) {
            return (_nano < tp._nano);
          }
        }
      }
    }
  }

  return false;
}

bool timePoint::operator>(const timePoint& tp) const
{
  return !((*this) < tp || (*this) == tp);
}

bool timePoint::operator==(const timePoint& tp) const
{
  return (_year == tp._year &&
          _month == tp._month &&
          _day == tp._day &&
          _hour == tp._hour &&
          _minute == tp._minute &&
          _second == tp._second &&
          _nano == tp._nano);
}

bool timePoint::operator!=(const timePoint& tp) const
{
  return !((*this) == tp);
}

bool timePoint::operator<=(const timePoint& tp) const
{
  return (*this) == tp || (*this) < tp;
}

bool timePoint::operator>=(const timePoint& tp) const
{
  return (*this) == tp || (*this) > tp;
}


