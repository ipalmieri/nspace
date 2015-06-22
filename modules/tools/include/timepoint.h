#ifndef __TIMEPOINT_H__
#define __TIMEPOINT_H__

#include "date.h"

namespace tools
{
   //////////////////////////////////////////////
   /// calendar time point in localtime (or UTC)
   /// prone to changes in system clock
   //////////////////////////////////////////////
   class timePoint
   {
     public:
     
      //timePoint() { (*this) = now(); }
      timePoint() {}
      ~timePoint() {}

      static timePoint now();
      static timePoint nowGMT();
      static timePoint nowUTC() { return nowGMT(); }
      timePoint toGMT();      

      std::string toStr() const;

      bool operator<(const timePoint &tp) const;
      bool operator>(const timePoint &tp) const;
      bool operator==(const timePoint &tp) const;
      bool operator!=(const timePoint &tp) const;
      bool operator<=(const timePoint &tp) const;
      bool operator>=(const timePoint &tp) const;

      date getDate() const { return date((uint) _day, (uint) _month, (uint) _year); }

      uint year() const { return _year; }
      uint month() const { return _month; } 
      uint day() const { return _day; }
      uint hour() const { return _hour; }
      uint minute() const { return _minute; }
      uint second() const { return _second; }
      uint millisecond() const { return _nano/1000000; }
      uint microsecond() const { return _nano/1000; }
      uint nanosecond() const { return _nano; }

      bool isDST() { return _isDST; }


     protected:
      
      //timePoint() {}
      timePoint(const tm &t, const uint64_t &nano);
      
      //helper function
      tm structTM() const;

      ushort _year;
      ushort _month;
      ushort _day;
      ushort _hour;
      ushort _minute;
      ushort _second;
      uint64_t _nano;
      bool _isDST;
   };
}

#endif //__TIMEPOINT_H__
