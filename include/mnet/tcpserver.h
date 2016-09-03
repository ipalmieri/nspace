#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include <string>
#include "tcpsocket.h"

#define NET_BACKLOG 1024

namespace tools {
class tcpServer
{
  CANNOT_COPY(tcpServer);

 public:

  tcpServer(const uint16_t& port, const uint& backlog=NET_BACKLOG);
  tcpServer(const std::string& addr, const uint16_t& port,
            const uint& backlog=NET_BACKLOG);
  ~tcpServer();

  tcpSocket* accept();
  void close();

  bool isGood()
  {
    return _isGood;
  }
  std::string lastStatus() const
  {
    return _lastStatus;
  }


 protected:

  void bind(const std::string& addr, const uint16_t& port);
  void listen(const uint& backlog);

  int _sock4;
  int _sock6;
  uint16_t _port;

  std::string _lastStatus;
  bool _isGood;
};
}

#endif //__TCPSERVER_H__
