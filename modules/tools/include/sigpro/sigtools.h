#ifndef __SIGTOOLS_H__
#define __SIGTOOLS_H__

#include "multimedia/csvtools.h"
#include "sigvec.h"
#include "logger.h"

namespace tools {
//////////////////////////////////////////////////////////
/// multiSignal Load and Store functions
//////////////////////////////////////////////////////////

template<typename T> void saveSignalCSV(const multiSignal<T>& sig,
                                        const std::string& filename,
                                        const char& delimiter)
{
  logger::logMsg("Saving signal data to file " + filename, LGR_DEBUG);

  csvWriter outfile(filename, delimiter);
  std::vector<std::string> tmp(sig.signalCount());

  for(unsigned i=0; i < sig.sampleCount() && outfile.isValid(); i++) {
    for(unsigned j=0; j < tmp.size(); j++) {
      std::ostringstream buff;
      buff << sig[i][j];
      tmp[j] = buff.str();
    }

    outfile.writeLine(tmp);
  }

  logger::logMsg("Saved " + std::to_string(sig.signalCount()) +
                 " series containing each " + std::to_string(sig.sampleCount()) +
                 " sample(s)", LGR_DEBUG);

}


template<typename T> multiSignal<T> loadSignalCSV(const std::string& filename,
    const char& delimiter)
{
  csvReader infile(filename, delimiter);
  std::vector< std::vector<std::string> > tmp;
  unsigned samcnt = 0;
  unsigned sigcnt = 0;

  logger::logMsg("Loading signal data from file " + filename, LGR_DEBUG);

  while (infile.isValid()) {
    std::vector<std::string> rline = infile.readLine();
    unsigned cols = rline.size();

    if (cols > 0) {
      tmp.push_back(rline);
      samcnt++;

      // found fewer or less columns than expected
      if (sigcnt != 0 && sigcnt != cols) {
        logger::logMsg("loadSignalCSV(): Line " +
                       std::to_string(infile.linesRead()) +
                       " contains " + std::to_string(cols) +
                       "fields, expected " + std::to_string(sigcnt), LGR_WARN);
      }

      sigcnt = (sigcnt == 0)? cols : std::min((unsigned) cols, sigcnt);
    }
  }


  if (samcnt > 0) {
    multiSignal<T> ret(samcnt, sigcnt);

    for (unsigned i=0; i < samcnt; i++) {
      for(unsigned j=0; j < sigcnt; j++) {
        T value;
        std::stringstream buff(tmp[i][j]);
        buff >> value;
        ret[i][j] = value;
      }
    }

    return ret;
  }

  logger::logMsg("Loaded " + std::to_string(sigcnt) +
                 " series containing each " + std::to_string(samcnt) +
                 " sample(s)", LGR_DEBUG);

  return multiSignal<T>(0, 0);
}
}

#endif //__SIGTOOLS_H__
