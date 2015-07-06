#ifndef __TCPSOCKET_H__
#define __TCPSOCKET_H__

#include <cstdint>
#include <cstdlib>
#include <string>
#include "adefs.h"

#define NET_ADDRLEN 1024     // maximum length of an IPv(4/6) address within a string
#define NET_RECVLEN 1024     // default buffer size for recvMsg
#define NET_SENDLEN 1024     // default buffer size for sendMsg
#define NET_RECVMAX 4096     // maximum buffer size for recvMsg
#define NET_SENDMAX 4096     // maximum buffer size for sendMsg


namespace tools
{
   typedef struct {
      char *payload;
      size_t length;
   } tcpMessage;

   typedef struct {
      std::string addr;
      uint16_t port;
   } socketHost;

   
   class tcpSocket // : public stateObject
   {
      CANNOT_COPY(tcpSocket);
      friend class tcpServer;

     public:

      tcpSocket();
      ~tcpSocket();

      void connect(const std::string &hostname, const uint16_t &port);
      void close();      

      bool sendMsg(tcpMessage *msg);
      bool recvMsg(tcpMessage *msg);

      bool isGood() { return _isGood; }
      bool isConnected();
      std::string lastStatus() const { return _lastStatus; }

      socketHost getLocalAddr();
      socketHost getRemoteAddr();


      void enableBlocking(const bool &val);
      void enableDebug(const bool &val);
      void enableLinger(const bool &val);
      void enableReuseAddr(const bool &val);
      void enableKeepAlive(const bool &val);
      
      void setTimeout(const uint32_t &timeout) { _socketTimeout = timeout; }
      void setRecvLen(const size_t &size);
      void setSendLen(const size_t &size);

     protected:
      
      int getSocketOpts();
      void setSocketOpts(const int &opts);


      bool _isGood;
      bool _isConnected;
      std::string _lastStatus;

      int _sockfd;

      uint32_t _socketTimeout;
      size_t _sendLength;
      size_t _recvLength;
 
   };
}

#endif //__TCPSOCKET_H__
