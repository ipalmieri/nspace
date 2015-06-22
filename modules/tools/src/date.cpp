#include <ctime>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cassert>
#include "timepoint.h"
#include "date.h"
#include "aux.h"

using namespace std;
using namespace tools;

date::date(const uint &d,
	   const uint &m,
	   const uint &y)
{
   day = (unsigned short) d;
   month = (unsigned short) m;
   year = (unsigned short) y;
}

date date::strToDate(const string &str)
{

   vector<string> tokens = Tokenize(str, "/");

   assert(tokens.size() == 3);

   unsigned short tmp[3];

   for (int i = 0; i < 3; i++)
   {
      istringstream s(tokens[i]);

      s >> tmp[i];
   }
   
   return date(tmp[0], tmp[1], tmp[2]);
}

date::~date()
{

}

std::string date::toStr()
{
   ostringstream output;

   output.fill('0');
   output << setw(2) << day;
   output << "/";
   output << setw(2) << month;
   output << "/";
   output << setw(2) << year;

   return output.str();
}

bool date::operator<(const date &date) const
{

   if (year < date.year)
   {
      return true;
   }
   else if (year == date.year)
   {
      if (month < date.month)
      {
	 return true;
      }
      else if (month == date.month)
      {
	 return (day < date.day);
      }
   }

   return false;
}


bool date::operator>(const date &date) const
{
   return !((*this) < date || (*this) == date);
}

bool date::operator==(const date &date) const
{
   return (year == date.year && month == date.month && day == date.day);
}

bool date::operator!=(const date &date) const
{
   return !((*this) == date);
}

bool date::operator<=(const date &date) const
{
   return (*this) == date || (*this) < date;
}

bool date::operator>=(const date &date) const
{
   return (*this) == date || (*this) > date;
}

string date::mysqlConvert(const string &date)
{
   vector<string> tokens = Tokenize(date, "-");

   if (tokens.size() == 3)
   {
      return tokens[2] + "/" + tokens[1] + "/" + tokens[0];
   }

   return date;
}

string date::mysqlRevert(const string &date)
{
   vector<string> tokens = Tokenize(date, "/");

   if (tokens.size() == 3)
   {
      return tokens[0] + "-" + tokens[1] + "-" + tokens[2];
   }

   return date;
}

string date::toMSQLStr()
{
   ostringstream output;

   output.fill('0');
   output << setw(2) << year;
   output << "-";
   output << setw(2) << month;
   output << "-";
   output << setw(2) << day;

   return output.str();
}

int date::daysFrom(const date &date) const
{
   int diff;

   diff = daysFromStart((*this)) - daysFromStart(date);

   return diff;
}

bool date::isValid() const
{
   if (year < 0) return false;

   if (month <= 0 || month > 12) return false;

   if (day <= 0 || day > daysOfMonth(month, year)) return false;

   return true;
}

unsigned int date::daysOfMonth(const unsigned int &month, const unsigned int &year)
{
   int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

   if (year < 0  || month < 0 || month > 12)
   {
      return 0;
   }

   if (isLeapYear(year)) days[1] = 29;

   return days[month - 1];
}


bool date::isLeapYear(const unsigned int &year)
{
   int y = year;

   if (year%100 == 0) y = year/100;

   if (y%4 == 0) return true;

   return false;

}

date date::today()
{
   timePoint tp = timePoint::now();

   return tp.getDate();
}

unsigned int date::daysFromStart(const date &date)
{
   unsigned int acc = 0;

   if (!date.isValid()) return 0;

   for (int i = 0; i < date.year; i++)
   {
      for (int j = 1; j <= 12; j++)
      {
	 acc += daysOfMonth(j, i);
      }
   }


   for (int i = 1; i <= date.month; i++)
   {
      acc += daysOfMonth(i, date.year);
   }

   acc += date.day;

   return acc;
}

date date::offsetDays(int days)
{
   date ret = *this;
   int offset = days;
	
   if (days > 0)
   {
      while (offset > 0)
      {
	 offset--;
	 ret.day++;
			
	 if (ret.day > daysOfMonth(ret.month, ret.year))
	 {
	    ret.day = 1;
	    ret.month++;
				
	    if (ret.month > 12)
	    {
	       ret.year++;
	       ret.month = 1;
	    }
	 }
      }
   }
   else if (days < 0)
   {
      while (offset < 0)
      {	
	 offset++;
	 ret.day--;
			
	 if (ret.day <= 0)
	 {
	    ret.month--;
	    if (ret.month <=0)
	    {
	       ret.month = 12;
	       ret.year--;
	    }
				
	    ret.day = daysOfMonth(ret.month, ret.year);
	 }
		
      }
   }
	
   return ret;
}
