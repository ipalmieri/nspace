#include <cstring>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <cerrno>
#include "aux.h"

using namespace std;
using namespace tools;

vector<string> tools::Tokenize(const string &str,
			const string &delimiters)
{
   vector<string> tokens;

   // Skip delimiters at beginning.
   string::size_type lastPos = str.find_first_not_of(delimiters, 0);
   // Find first "non-delimiter".
   string::size_type pos     = str.find_first_of(delimiters, lastPos);

   while (string::npos != pos || string::npos != lastPos)
   {
      // Found a token, add it to the vector.
      tokens.push_back(str.substr(lastPos, pos - lastPos));
      // Skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of(delimiters, pos);
      // Find next "non-delimiter"
      pos = str.find_first_of(delimiters, lastPos);
   }

   return tokens;
}

void tools::trim(string &str)
{
   string::size_type pos = str.find_last_not_of(' ');
 
   if(pos != string::npos) 
   {
      str.erase(pos + 1);
   
      pos = str.find_first_not_of(' ');
   
      if(pos != string::npos) str.erase(0, pos);
   }
   else
   { 
      str.erase(str.begin(), str.end());
   }
}


long tools::cint(const double &x)
{
   if (fabs(x - floor(x)) >= 0.5f)
      return (x >= 0)? ceil(x) : floor(x);
   else
      return (x < 0)? ceil(x) : floor(x);
}

unsigned tools::randU(const unsigned &start, const unsigned &end)
{
   //srand((unsigned) time(NULL));

   if (end < start) return 0;

   unsigned ret = cint(((double)(end - start)) * (double)rand()/RAND_MAX);
   ret += start;

   return ret;
}  

double tools::gaussRand(const double &mean, const double &stdev)
{
   static double v1, v2, s;
   static int phase = 0;
   double x;

   if (phase == 0) {
      do {
	 v1 = (double)rand() / RAND_MAX;
	 v2 = (double)rand() / RAND_MAX;

	 v1 = 2 * v1 - 1;
	 v2 = 2 * v2 - 1;
	 s = v1 * v1 + v2 * v2;
      } while (s >= 1 || s == 0);

      x = v1 * sqrt(-2 * log(s) / s);
   } else
      x = v2 * sqrt(-2 * log(s) / s);

   phase = 1 - phase;

   return x*stdev + mean;
}

string tools::istr(const int &x)
{
    ostringstream temp;

    temp << x;

    return temp.str();
}

// return last error
// fix this: should use strerror_r to be thread safe
string tools::lastError() 
{
   char *e = strerror(errno);

   return e ? e : "NULL";
}
