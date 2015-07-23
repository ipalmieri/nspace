#ifndef __TCPMSG_H__
#define __TCPMSG_H__

#include <algorithm>

namespace tools
{
   class tcpMessage
   {
     public:
      
      tcpMessage(const size_t &maxlen);
      ~tcpMessage();
      
      tcpMessage(const tcpMessage &msg);
      tcpMessage &operator=(const tcpMessage &msg);
      
      inline char *payload() const { return _payload; };
      inline size_t length() const { return _length; };
      inline size_t maxlen() const { return _maxlen; };

      void setLength(const size_t &len) { _length = std::min(len, _maxlen); };
      void readString(const std::string &str);
      std::string writeString() const;

     protected:

      char *_payload;
      size_t _length;
      size_t _maxlen;

      void init(const size_t &maxlen);
      void stop();
      void copy(const tcpMessage &msg);
   };
}

#endif //__TCPMSG_H__
