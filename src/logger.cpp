#include "logger.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include "timepoint.h"


using namespace std;
using namespace tools;

//////////////////////////////////////////////////////////////
// logObject class functions

std::string logObject::levelToStr(const logLevel& level)
{
  switch (level) {
    case LGR_TRACE:
      return std::string("TRACE");
      break;
    case LGR_DEBUG:
      return std::string("DEBUG");
      break;
    case LGR_INFO:
      return std::string("INFO");
      break;
    case LGR_WARNING:
      return std::string("WARNING");
      break;
    case LGR_ERROR:
      return std::string("ERROR");
      break;
    case LGR_ALL:
      return std::string("");
      break;
    default:
      break;
  }

  return std::string("LGR_NOT_DEFINED");
}

std::string logObject::formatMsgLong(const string& msg, const string& sname,
                                     const logLevel& level)
{
  //time_t t = time(NULL);
  //struct tm * now = localtime(& t);

  timePoint tp = timePoint::now();

  ostringstream tstr;
  tstr << setw(2) << setfill('0') << tp.hour();
  tstr << ":" << setw(2) << setfill('0') << tp.minute();
  tstr << ":" << setw(2) << setfill('0') << tp.second();
  tstr << "." << setw(6) << setfill('0') << tp.microsecond();
  tstr << " ";

  tstr << setw(2) << setfill('0') << tp.day();
  tstr << "/" << setw(2) << setfill('0') << tp.month();
  tstr << "/" << setw(4) << setfill('0') << tp.year();
  tstr << " ";

  string tstamp = tstr.str();

  string lname = (sname.size() > 0)? "[" + sname + "] " :  "";

  string lstr = levelToStr(level);

  lstr = (lstr.size() > 0) ? lstr + ": " : "";


  return tstamp + lname + lstr + msg;
}

string logObject::formatMsgShort(const string& msg, const string& sname,
                                 const logLevel& level)
{
  string lname = (sname.size() > 0 &&
                  sname != _LOGGER_DSNAME)? "[" + sname + "] " :  "";

  return lname + msg;
}

//////////////////////////////////////////////////////////////
// logFile methods

void logFile::printMsg(const string& msg, const string& sname,
                       const logLevel& level)
{

  string message = formatMsgLong(msg, sname, level);

  if (message[message.size() - 1] != '\n') {
    message += '\n';
  }


  if (!_outf.is_open()) {
    _outf.open(_filename.c_str(), std::ios::app);
  }

  if (_outf.good()) {
    _outf << message;

    _outf.flush();
  }
}

//////////////////////////////////////////////////////////////
// logSyslog methods

void logSyslog::printMsg(const string& msg, const string& sname,
                         const logLevel& level)
{

  string message = formatMsgShort(msg, sname, level);

  // write to syslog
  int syslevel = levelToSyslog(level);

  syslog(_facility|syslevel, message.c_str());
}

int logSyslog::levelToSyslog(const logLevel& level)
{

  switch (level) {
    case LGR_TRACE:
      return LOG_DEBUG;
    case LGR_DEBUG:
      return LOG_DEBUG;
    case LGR_INFO:
      return LOG_INFO;
    case LGR_WARNING:
      return LOG_WARNING;
    case LGR_ERROR:
      return LOG_ERR;
    default:
      break;
  }

  return LOG_DEBUG;
}

//////////////////////////////////////////////////////////////
// logTerminal methods

void logTerminal::printMsg(const string& msg, const string& sname,
                           const logLevel& level)
{
  string message = formatMsgLong(msg, sname, level);

  if (level <= LGR_ERROR) {
    cerr << message << endl;
  } else {
    cout << message << endl;
  }
}

//////////////////////////////////////////////////////////////
// logSource methods

logSource::logSource(const string& sname)
{
  name = sname;
  level = LGR_ALL;
}

logSource::~logSource()
{
  for (vector<logObject*>::iterator it = objects.begin(); it != objects.end();
       ++it) {
    delete *it;
  }
}

//////////////////////////////////////////////////////////////
// logger singleton class methods

logger::logger()
{
  setDefaultSource();

  _useSyslog = false;
}

logger::~logger()
{
  for (lmap::iterator it = _sources.begin(); it != _sources.end(); ++it) {
    delete it->second;
  }

  if (_useSyslog) {
    closelog();
  }
}

void logger::setDefaultSource(const string& sname)
{
  if (sname.size() > 0) {
    _defaultSource = sname;
  } else {
    _defaultSource = _LOGGER_DSNAME;
  }
}

logSource* logger::getSource(const string& sname)
{
  string nsname = (sname.size() > 0)? sname : _defaultSource;

  lmap::iterator it = _sources.find(nsname);

  if (it == _sources.end() || it->second == NULL) {
    logSource* nsrc = new logSource(nsname);

    _sources[nsname] = nsrc;

    return nsrc;
  }

  return it->second;
}

void logger::deleteSource(const string& sname)
{
  lmap::iterator it;

  do {

    it = _sources.find(sname);

    if (it != _sources.end()) {
      delete it->second;

      _sources.erase(it);
    }

  } while (it != _sources.end());
}

void logger::setSourceLevel(const logLevel& level, const string& sname)
{
  logSource* nsrc = getSource(sname);

  if (nsrc) {
    nsrc->level = level;
  }
}


void logger::addFile(const string& filename, const string& sname)
{
  if (filename.length() <= 0) {
    return;
  }

  logSource* nsrc = getSource(sname);

  logFile* nfile = new logFile(filename);

  nsrc->objects.push_back(nfile);
}


void logger::enableSyslog(const int& facility, const string& sname)
{
  logSource* nsrc = getSource(sname);

  logSyslog* nlog = new logSyslog(facility);

  nsrc->objects.push_back(nlog);

  if (!_useSyslog) {
    openlog(NULL, LOG_NDELAY | LOG_NOWAIT | LOG_PID, _LOGGER_DEFAC);

    _useSyslog = true;
  }
}

void logger::enableTerminal(const string& sname)
{
  logSource* nsrc = getSource(sname);

  logTerminal* nlog = new logTerminal();

  nsrc->objects.push_back(nlog);
}

void logger::logServerMsg(const string& msg, const string& sname,
                          const logLevel& level)
{
  logSource* nsrc  = getSource(sname);

  if (level <= nsrc->level && nsrc->level != LGR_NONE) {
    // bcast the message to target log objects
    vector<logObject*>::iterator vit = (nsrc->objects).begin();

    while (vit != nsrc->objects.end()) {
      //cout << msg << " " << nsrc->name << endl;

      (*vit)->printMsg(msg, nsrc->name, level);

      ++vit;
    }
  }
}


void logger::startLog(const string& filename, const string& sname,
                      const logLevel& level)
{
  logger* ls = logServer::getInstance();

  ls->setDefaultSource(sname);
  ls->addFile(filename, sname);
  ls->setSourceLevel(level, sname);
}

void logger::startSyslog(const string& sname, const logLevel& level)
{
  logger* ls = logServer::getInstance();

  ls->setDefaultSource(sname);
  ls->enableSyslog(_LOGGER_DEFAC, sname);
  ls->setSourceLevel(level, sname);
}

void logger::startTerminal(const string& sname, const logLevel& level)
{
  logger* ls = logServer::getInstance();

  ls->setDefaultSource(sname);
  ls->enableTerminal(sname);
  ls->setSourceLevel(level, sname);
}

void logger::logMsg(const string& msg, const logLevel& level)
{
  logMsg(msg, "", level);
}

void logger::logMsg(const string& msg, const string& sname,
                    const logLevel& level)
{
  logger* ls = logServer::getInstance();

  ls->logServerMsg(msg, sname, level);
}

// return logLevel given a verbosity
logLevel logger::intToLevel(const int& value)
{
  return std::max(_LGR_MAXP, std::min(_LGR_MINP, (logLevel) value));
}
