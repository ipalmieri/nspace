#ifndef __MULTIMEDIA_H_
#define __MULTIMEDIA_H__

#include <vector>
#include <string>
#include <fstream>
#include "multimedia/imagemap.h"


namespace tools
{

   // JPEG manipulation functions

   void writeJPEG(const char *filename, const imageMap &img);
   void readJPEG(const char *filename, imageMap &img);


   //////////////////////////////////////////////////////////////
   /// CSV file reading class
   //////////////////////////////////////////////////////////////
   class csvReader
   {
     public:
      csvReader(const std::string &filename, const std::string &delimiter=";");
      ~csvReader();

      inline unsigned linesRead() { return _lread; }
      inline std::string filename() { return _filename; }
      std::vector<std::string> firstLine() { return _firstLine; }

      std::vector<std::string> getLine();
      bool isValid() { return _infile.good(); }
      
      void openFile();
      void gotoLine(const unsigned &line);
      
     protected:
      unsigned _lread;
      std::vector<std::string> _firstLine;
      std::string _filename;
      std::string _delimiter;

      std::ifstream _infile;
   };

   std::vector< std::vector<std::string> > readCSV(const std::string &filename, const std::string &delimiter = ";");

}

#endif //__MULTIMEDIA_H__
