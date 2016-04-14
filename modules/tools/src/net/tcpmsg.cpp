#include "net/tcpmsg.h"

#include <cstring>

using namespace std;
using namespace tools;


tcpMessage::tcpMessage(const size_t &maxlen)
{
   _payload = NULL;
   _maxlen = 0;
   _length = 0;

   init(maxlen);
}


tcpMessage::~tcpMessage()
{
   stop();
}


tcpMessage::tcpMessage(const tcpMessage &msg)
{
   _payload = NULL;
   _maxlen = 0;
   _length = 0;

   copy(msg);
}  


tcpMessage &tcpMessage::operator=(const tcpMessage &msg)
{
   copy(msg);

   return *this;
}

void tcpMessage::init(const size_t &maxlen)
{
   if (maxlen == 0)
   {
      stop();
      return;
   }

   char *btmp = (char *) realloc(_payload, maxlen*sizeof(char));

   if (btmp)
   {
      _payload = btmp;
      _maxlen = maxlen;
   }
}


void tcpMessage::stop()
{
   if (_payload)
   {
      free(_payload);

      _payload = NULL;
      _maxlen = 0;
      _length = 0;
   }
}


void tcpMessage::copy(const tcpMessage &msg)
{
   init(msg.maxlen());

   char *ptr = (char *) msg.payload();

   for(uint i = 0; i < _maxlen && i < msg.length(); i++)
      
	  _payload[i] = ptr[i];

}


void tcpMessage::readString(const string &str)
{
   _length = min(str.length(), _maxlen);

   strncpy(_payload, str.c_str(), _length);
}
   
string tcpMessage::writeString() const
{
   return string(_payload, _length);
}
