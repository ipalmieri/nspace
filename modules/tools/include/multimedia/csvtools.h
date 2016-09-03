#ifndef __CSVTOOLs_H__
#define __CSVTOOLs_H__


#include <vector>
#include <string>
#include <fstream>

#define CSV_DEF_DELIM ','

namespace tools {
//////////////////////////////////////////////////////////////
/// CSV file reading class
//////////////////////////////////////////////////////////////
class csvReader
{
 public:
  csvReader(const std::string& filename, const std::string& delimiter);
  csvReader(const std::string& filename, const char& delimiter=CSV_DEF_DELIM)
    :csvReader(filename, std::string(1, delimiter)) {}
  ~csvReader();

  inline unsigned linesRead() const
  {
    return _lread;
  }
  inline std::string filename() const
  {
    return _filename;
  }
  std::vector<std::string> firstLine()
  {
    return _firstLine;
  }

  std::vector<std::string> getLine();
  inline std::vector<std::string> readLine()
  {
    return getLine();
  }
  bool isValid()
  {
    return _infile.good();
  }

  void openFile();
  void gotoLine(const unsigned& line);

 protected:
  unsigned _lread;
  std::vector<std::string> _firstLine;
  std::string _filename;
  std::string _delimiter;

  std::ifstream _infile;
};

//////////////////////////////////////////////////////////////
/// CSV file writing class
//////////////////////////////////////////////////////////////
class csvWriter
{
 public:
  csvWriter(const std::string& filename,
            const std::string& delimiter,
            bool append=false);
  csvWriter(const std::string& filename,
            const char& delimiter=CSV_DEF_DELIM,
            bool append=false) : csvWriter(filename, std::string(1, delimiter), append) {}

  ~csvWriter();

  inline unsigned linesWritten() const
  {
    return _lwritten;
  }
  inline std::string filename() const
  {
    return _filename;
  }

  void writeLine(const std::vector<std::string>& line);
  bool isValid()
  {
    return _oufile.good();
  }

  void openFile();

 protected:
  unsigned _lwritten;
  std::string _filename;
  std::string _delimiter;
  bool _append;

  std::ofstream _oufile;
};


//////////////////////////////////////////////////////////////
/// Auxiliary functions
//////////////////////////////////////////////////////////////

std::vector< std::vector<std::string> > readCSV(const std::string& filename,
    const std::string& delimiter = ",");
}

#endif //__CSVTOOLs_H__
