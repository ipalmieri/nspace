#include "multimedia.h"
#include "aux.h"
#include <limits>

using namespace tools;
using namespace std;

csvReader::csvReader(const std::string &filename, const std::string &delimiter)
{
   _lread = 0;
   _filename = filename;
   _delimiter = delimiter;
}

csvReader::~csvReader()
{
   if (_infile.is_open())
   {
      _infile.close();
   }
}

vector<string> csvReader::getLine()
{
   vector<string> ret;

   string line;
   
   openFile();

   getline(_infile, line);

   if (line.size() > 0)
   {
      ret = Tokenize(line, _delimiter);

      if (_lread == 0) _firstLine = ret;

      _lread++;
   }
   
   return ret;
}

void csvReader::openFile()
{
   if (!_infile.is_open())
   {
      //fix this - must throw exception
      _infile.open(_filename.c_str());
      _lread = 0;
   }
}

void csvReader::gotoLine(const unsigned &line)
{
   openFile();
   
   _infile.seekg(std::ios::beg);
    for (unsigned i = 0; i < line; i++){
        _infile.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
}

vector< vector<string> > tools::readCSV(const string &filename, const string &delimiter)
{
   vector< vector<string> > ret;
   ifstream infile;
   string line;
   int linec;

   infile.open(filename.c_str());
   
   if (infile.is_open())
   {
      linec = 1;
      while (infile.good())
      {
	 getline(infile, line);
	 
	 if (line.size() > 0)
	 {	    
	    
	    ret.push_back(Tokenize(line, delimiter));
	  
	    linec++;
	 }
      }
   }

   infile.close();

   return ret;
}
