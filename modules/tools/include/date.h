#ifndef __DATE_H__
#define __DATE_H__

#include <string>

namespace tools
{
   ///////////////////////////////////////////////
   /// Simple DD/MM/YY class with arithmetic
   ///////////////////////////////////////////////
   class date
   {
     public:
      date(const uint &day, const uint &month, const uint &year);
      ~date();

      //operações de conversão - tirar daqui
      static std::string mysqlConvert(const std::string &date);
      static std::string mysqlRevert(const std::string &date);

      std::string toStr();
      std::string toMSQLStr();

      ushort day;
      ushort month;
      ushort year;

      bool operator<(const date &date) const;
      bool operator>(const date &date) const;
      bool operator==(const date &date) const;
      bool operator!=(const date &date) const;
      bool operator<=(const date &date) const;
      bool operator>=(const date &date) const;

      int daysFrom(const date &date) const;
      date offsetDays(int days); 
		
      bool isValid() const;

      static unsigned int daysOfMonth(const unsigned int &month, const unsigned int &year);
      static bool isLeapYear(const unsigned int &year);

      static date today();
      static date strToDate(const std::string &str);
		

     protected:

      static unsigned int daysFromStart(const date &date);

   };


}

#endif
