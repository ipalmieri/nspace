#include "mnet/tcpserver.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "aux.h"

using namespace std;
using namespace tools;


tcpServer::tcpServer(const uint16_t& port, const uint& backlog)
{
  _port = port;
  _sock4 = -1;
  _sock6 = -1;
  _isGood = true;

  bind("", port);
  listen(backlog);
}


tcpServer::tcpServer(const string& addr, const uint16_t& port,
                     const uint& backlog)
{
  _port = port;
  _sock4 = -1;
  _sock6 = -1;
  _isGood = true;

  bind(addr, port);
  listen(backlog);
}


tcpServer::~tcpServer()
{
  close();
}


void tcpServer::close()
{
  if (_sock4 != -1) {
    ::close(_sock4);
  }

  if (_sock6 != -1) {
    ::close(_sock6);
  }
}


tcpSocket* tcpServer::accept()
{
  int retval;
  int newfd;
  tcpSocket* retsock = NULL;
  fd_set acset;

  if (_sock4 == -1 && _sock6 == -1) {
    _lastStatus = "Accept with no socket created";
    return NULL;
  }

  FD_ZERO(&acset);

  if (_sock4 != -1) {
    FD_SET(_sock4, &acset);
  }

  if (_sock6 != -1) {
    FD_SET(_sock6, &acset);
  }


  retval = select(max(_sock4, _sock6) + 1, &acset, NULL, NULL, NULL);

  if (retval == -1) {
    if (errno != EINTR) {
      _lastStatus = tools::funcLastError("select");
      _isGood = false;
    }
  } else if (retval == 0) {
    _lastStatus = "select(): accept timeout";
  } else {
    if (_sock4 != -1 && FD_ISSET(_sock4, &acset)) {
      newfd = ::accept(_sock4, NULL, NULL);
    } else if (_sock6 != -1 && FD_ISSET(_sock6, &acset)) {
      newfd = ::accept(_sock6, NULL, NULL);
    } else {
      _lastStatus = "select(): returned with no valid fd";
      return NULL;
    }

    retsock = new tcpSocket();
    retsock->connectSocket(newfd);
  }

  return retsock;
}

void tcpServer::bind(const string& addr, const uint16_t& port)
{
  bool has_ipv4 = false;
  bool has_ipv6 = false;
  int ret4, ret6;
  struct sockaddr_in addr4;
  struct sockaddr_in6 addr6;


  // Configure addresses
  bzero(&addr4, sizeof(addr4));
  bzero(&addr6, sizeof(addr6));

  addr4.sin_family = AF_INET;
  addr4.sin_port = htons(port);
  addr6.sin6_family = AF_INET6;
  addr6.sin6_port = htons(port);


  if (addr.empty()) {
    addr4.sin_addr.s_addr = INADDR_ANY;
    addr6.sin6_addr = in6addr_any;

    has_ipv4 = true;
    has_ipv6 = true;
  } else {

    ret4 = inet_pton(AF_INET, addr.c_str(), &(addr4.sin_addr));

    if (ret4 < 0) {
      _lastStatus = tools::funcLastError("inet_pton");
    } else if (ret4 > 0) {
      has_ipv4 = true;
    }


    ret6 = inet_pton(AF_INET6, addr.c_str(), &(addr6.sin6_addr));

    if (ret6 < 0) {
      _lastStatus = tools::funcLastError("inet_pton");
    } else if (ret6 > 0) {
      has_ipv6 = true;
    }

  }



  // Create sockets and binds
  if (has_ipv4) {
    _sock4 = socket(AF_INET, SOCK_STREAM, 0);

    if (_sock4 != -1) {
      ret4 = ::bind(_sock4, (struct sockaddr*) &addr4, sizeof(addr4));

      if (ret4 < 0) {
        has_ipv4 = false;
        ::close(_sock4);
        _sock4 = -1;

        _lastStatus = tools::funcLastError("bind");
      }
    } else {
      _lastStatus = tools::funcLastError("socket");

      has_ipv4 = false;
    }
  }



  if (has_ipv6) {
    _sock6 = socket(AF_INET6, SOCK_STREAM, 0);


    if (_sock6 != -1) {

      if (has_ipv4) {
        int on = 1;
        int r = setsockopt(_sock6, IPPROTO_IPV6, IPV6_V6ONLY, &on, sizeof(on));

        if (r < 0) {
          _lastStatus = tools::funcLastError("setsockopt");
        }

      }

      ret6 = ::bind(_sock6, (struct sockaddr*) &addr6, sizeof(addr6));

      if (ret6 < 0) {
        has_ipv6 = false;
        ::close(_sock6);
        _sock6 = -1;

        _lastStatus = tools::funcLastError("bind");
      }

    } else {
      _lastStatus = tools::funcLastError("socket");

      has_ipv6 = false;
    }

  }


  if(!has_ipv4 && !has_ipv6) {
    _isGood = false;
    return;
  }
}

void tcpServer::listen(const uint& backlog)
{
  int ret;

  if (_sock4 != -1) {
    ret = ::listen(_sock4, backlog);

    if (ret < 0) {
      _lastStatus = tools::funcLastError("listen");
      _isGood = false;

      ::close(_sock4);
      _sock4 = -1;

      return;
    }
  }

  if (_sock6 != -1) {
    ret = ::listen(_sock6, backlog);

    if (ret < 0) {
      _lastStatus = tools::funcLastError("listen");
      _isGood = false;

      ::close(_sock6);
      _sock6 = -1;

      return;
    }
  }
}


