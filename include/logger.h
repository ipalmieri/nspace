#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <vector>
#include <map>
#include <syslog.h>
#include <iomanip> 
#include <iostream> 
#include <fstream>
#include "singleton.h"

#define _LOGGER_DSNAME "default"
#define _LOGGER_DEFAC LOG_LOCAL0

namespace tools
{
   //////////////////////////////////////////////
   /// log level values
   /// MINP and MAXP are only kept to mark limits
   //////////////////////////////////////////////
   enum logLevel 
   {
      _LGR_MINP = 4,
      _LGR_MAXP = 0,

      LGR_ALL = 99,
      LGR_DEBUG = 4, 
      LGR_INFO = 3, 
      LGR_WARNING = 2, 
      LGR_ERROR = 1,
      LGR_NONE = 0
   }; 

   std::string formatLong(const std::string &msg, const std::string &name, const logLevel &level);
   std::string formatShort(const std::string &msg, const std::string &name, const logLevel &level);


   //////////////////////////////////////////////
   /// Abstract log output lobject
   //////////////////////////////////////////////
   class logObject
   {
     public:

      logObject() {}
      virtual ~logObject() {};
      
      virtual void printMsg(const std::string &msg, 
			    const std::string &name="", 
			    const logLevel &level=LGR_ALL) = 0;
   };


   //////////////////////////////////////////////
   /// Log to file object
   //////////////////////////////////////////////
   class logFile : public logObject
   {
     public:

      logFile(const std::string &filename) { _filename = filename; }
      ~logFile() { if (_outf.is_open()) { _outf.close(); }}
      
      void printMsg(const std::string &msg, const std::string &name, const logLevel &level);
      inline std::string getFilename() const { return _filename; }
      
     protected:

      std::string _filename;
      std::ofstream _outf;
   };


   //////////////////////////////////////////////
   /// Log to syslog object
   //////////////////////////////////////////////
   class logSyslog : public logObject
   {
     public:

      logSyslog(const int &facility=_LOGGER_DEFAC) { _facility = facility; }
      ~logSyslog() {}

      void printMsg(const std::string &msg, const std::string &name, const logLevel &level);

     protected:

      static int levelToSyslog(const logLevel &level);

      int _facility;
   };

   
   //////////////////////////////////////////////
   /// Log to terminal object (stdout/stderr)
   //////////////////////////////////////////////
   class logTerminal : public logObject
   {
     public:

      logTerminal() {}
      ~logTerminal() {}

      void printMsg(const std::string &msg, const std::string &name, const logLevel &level);
   };


   //////////////////////////////////////////////
   /// Log source 
   /// just a name from where logs originate
   //////////////////////////////////////////////
   class logSource 
   {
     private:
      friend class logger;

      logSource(const std::string &sname);
      ~logSource();

      std::string name;
      logLevel level;
      std::vector<logObject *> objects;
   };

  
   //////////////////////////////////////////////
   /// Log router and server
   /// List of sources and objects
   //////////////////////////////////////////////
   class logger
   {
     public:
      friend class tools::singleton<logger>;

      void setDefaultSource(const std::string &sname="");
      void setSourceLevel(const logLevel &level, const std::string &sname="");

      void addFile(const std::string &filename, const std::string &sname="");
      void enableSyslog(const int &facility, const std::string &sname="");
      void enableTerminal(const std::string &sname="");

      void logServerMsg(const std::string &msg, 
			const std::string &sname="", 
			const logLevel &level=LGR_ALL);

      //helper functions
      static std::string levelToStr(const logLevel &level);
      static logLevel intToLevel(const int &value);



      // under normal operation only these functions below should be used
      static void startLog(const std::string &filename, 
			   const std::string &sname="",
			   const logLevel &level=LGR_ALL);

      static void startSyslog(const std::string &sname="",
			      const logLevel &level=LGR_ALL);

      static void startTerminal(const std::string &sname="",
				const logLevel &level=LGR_ALL);



      // generic log message - should be the choice to send messages
      static void logMsg(const std::string &msg, const logLevel &level=LGR_ALL);      
      
      static void logMsg(const std::string &msg,
			 const std::string &sname,
			 const logLevel &level=LGR_ALL);

     protected:
      logger();
      ~logger();
 
      logSource *getSource(const std::string &sname);
      void deleteSource(const std::string &sname);
    
      typedef std::map<std::string, logSource *> lmap;
      lmap _sources;
      std::string _defaultSource;
      bool _useSyslog;
   };



   typedef tools::singleton<logger> logServer;
}

#endif //__LOGGER_H__
