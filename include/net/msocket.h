#ifndef __MSOCKET_H__
#define __MSOCKET_H__

#include <cstdint>
#include <cstdlib>

namespace tools
{
   class tcpSocket
   {
     public:
      tcpSocket();
      ~tcpSocket();

      void setBlocking(const bool &enable);
      void setBufferSize(const size_t &size);
      
      void connect();
      bool write();
      bool read();
      void close();
      


     protected:
      
      uint16_t _port;
      bool _isBlocking;
      char *_buffer;
      
   };


   class tcpServer
   {

   };



   class udpSocket
   {


   };


   class udpServer
   {

   };

}

#endif //__MSOCKET_H__
