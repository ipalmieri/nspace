#ifndef __AUX_H__
#define __AUX_H__

#include <string>
#include <vector>

//auxiliary functions and definitions

namespace tools
{
   //string tokenizer
   std::vector<std::string> Tokenize(const std::string& str,
				     const std::string& delimiters = " ");


   //string trim
   void trim(std::string &str);

   //double to int
   long cint(const double &x);

   //int to string
   std::string istr(const int &x);

   //random unsigned integer
   unsigned randU(const unsigned &start, const unsigned &end);

   //Marsaglia method - gaussian random number generator
   double gaussRand(const double &mean=0.0f, const double &stdev=1.0f);

   //Return srerror(errno) as string
   std::string lastError();
   

}

#endif
