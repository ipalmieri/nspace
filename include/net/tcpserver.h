#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include <cstdint>
#include <cstdlib>
#include <string>
#include "tcpsocket.h"


namespace tools
{
   class tcpServer
   {
      tcpServer(const uint16_t &port);
      ~tcpServer();

      tcpSocket *accept();
      void close();
      
     protected:
      
      void bind();
      void listen();
         
     private:

      // assign and copy
      
   };
}

#endif //__TCPSERVER_H__
