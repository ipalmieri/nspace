#ifndef __TCPSOCKET_H__
#define __TCPSOCKET_H__

#include <cstdint>
#include <cstdlib>
#include <string>
#include "adefs.h"
#include "tcpmsg.h"

#define NET_ADDRLEN 1024     // maximum length of an IPv(4/6) address within a string
#define NET_RECVLEN 1024     // default buffer size for recvMsg
#define NET_SENDLEN 1024     // default buffer size for sendMsg
#define NET_RECVMAX 4096     // maximum buffer size for recvMsg
#define NET_SENDMAX 4096     // maximum buffer size for sendMsg
#define NET_DLINGER 0        // secons to wait for close() 

namespace tools {
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

  void connect(const std::string& hostname, const uint16_t& port);
  void close();
  inline void disconnect()
  {
    close();
  }

  size_t sendMsg(tcpMessage* msg);
  size_t recvMsg(tcpMessage* msg);

  inline bool isGood()
  {
    return _isGood;
  }
  inline bool isConnected()
  {
    return _isConnected && _isGood;
  }
  std::string lastStatus() const
  {
    return _lastStatus;
  }

  socketHost getLocalAddr();
  socketHost getRemoteAddr();

  void enableBlocking(const bool& val);
  void enableDebug(const bool& val);
  void enableLinger(const bool& val, const int& secs = NET_DLINGER);
  void enableReuseAddr(const bool& val);
  void enableKeepAlive(const bool& val);

  void setRecvLen(const size_t& size);
  void setSendLen(const size_t& size);

 protected:

  void connectSocket(const int& fd);
  int getSocketOpts();
  void setSocketOpts(const int& opts);

  bool _isGood;
  bool _isConnected;
  std::string _lastStatus;
  size_t _sendLength;
  size_t _recvLength;

  int _sockfd;
};
}

#endif //__TCPSOCKET_H__
