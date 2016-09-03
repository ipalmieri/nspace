#include "multimedia/csvtools.h"
#include "logger.h"
#include "aux.h"
#include <limits>

using namespace std;
using namespace tools;

//////////////////////////////////////////////////////////////
/// CSV file reading class
//////////////////////////////////////////////////////////////

csvReader::csvReader(const std::string& filename, const std::string& delimiter)
{
  _lread = 0;
  _filename = filename;
  _delimiter = delimiter;
}

csvReader::~csvReader()
{
  if (_infile.is_open()) {
    _infile.close();
  }
}

vector<string> csvReader::getLine()
{
  vector<string> ret;

  string line;

  openFile();

  getline(_infile, line);

  if (line.size() > 0) {
    ret = Tokenize(line, _delimiter);

    if (_lread == 0) {
      _firstLine = ret;
    }

    _lread++;
  }

  return ret;
}

void csvReader::openFile()
{
  if (!_infile.is_open()) {
    _infile.open(_filename.c_str());
    _lread = 0;

    if (!_infile.is_open() || !_infile.good()) {
      logger::logMsg(funcLastError(__FUNCTION__), LGR_ERROR);
    }
  }
}

void csvReader::gotoLine(const unsigned& line)
{
  openFile();

  _infile.seekg(std::ios::beg);
  for (unsigned i = 0; i < line; i++) {
    _infile.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
  }
}

//////////////////////////////////////////////////////////////
/// CSV file writing class
//////////////////////////////////////////////////////////////

csvWriter::csvWriter(const std::string& filename,
                     const std::string& delimiter,
                     bool append)
{
  _lwritten = 0;
  _filename = filename;
  _delimiter = delimiter;
  _append = append;
}

csvWriter::~csvWriter()
{
  if (_oufile.is_open()) {
    _oufile.close();
  }
}

void csvWriter::writeLine(const std::vector<std::string>& line)
{
  openFile();

  uint scount = line.size();

  for (uint i=0; i < scount && _oufile.good(); i++) {
    _oufile << line[i];

    if (i < scount - 1) {
      _oufile << _delimiter;
    }
  }

  _oufile << endl;

  _lwritten++;
}

void csvWriter::openFile()
{
  if (!_oufile.is_open()) {
    //fix this - must throw exception
    if (_append) {
      _oufile.open(_filename.c_str(), std::ofstream::out | std::ofstream::app);
    } else {
      _oufile.open(_filename.c_str());
    }

    _lwritten = 0;

    if (!_oufile.is_open() || !_oufile.good()) {
      logger::logMsg(funcLastError(__FUNCTION__), LGR_ERROR);
    }
  }
}


//////////////////////////////////////////////////////////////
/// Auxiliary functions
//////////////////////////////////////////////////////////////

vector< vector<string> > tools::readCSV(const string& filename,
                                        const string& delimiter)
{
  vector< vector<string> > ret;
  ifstream infile;
  string line;
  int linec;

  infile.open(filename.c_str());

  if (infile.is_open()) {
    linec = 1;
    while (infile.good()) {
      getline(infile, line);

      if (line.size() > 0) {

        ret.push_back(Tokenize(line, delimiter));

        linec++;
      }
    }
  }

  infile.close();

  return ret;
}


