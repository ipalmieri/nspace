#include "net/tcpsocket.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "aux.h"

using namespace std;
using namespace tools;


tcpSocket::tcpSocket()
{
   _isGood = true;
   _isConnected = false;
   _sockfd = -1;

   setRecvLen(NET_RECVLEN);
   setSendLen(NET_SENDLEN);
}


tcpSocket::~tcpSocket()
{
   close();
}  


// fix this: should do non_blockin connect, with timeout
void tcpSocket::connect(const std::string &hostname, const uint16_t &port)
{
   struct addrinfo *result, *rp;
   struct addrinfo hints;
   int sock, ret;

   if (_isConnected || _sockfd > 0) close();

   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_family = AF_UNSPEC;
   // fix this: BSD compatibility check
   //hints.ai_flags = (AI_V4MAPPED|AI_ADDRCONFIG);
   hints.ai_flags = 0;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_protocol = 0;    

   ret = getaddrinfo(hostname.c_str(), std::to_string(port).c_str(), &hints, &result);

   if (ret != 0) 
   {
      _lastStatus = "getaddrinfo(): " + string(gai_strerror(ret));
      return;
   }

   for (rp = result; rp != NULL && !_isConnected; rp = rp->ai_next) 
   {
      sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

      if (sock != -1) {

	 if (::connect(sock, rp->ai_addr, rp->ai_addrlen) != -1)
	 {
	    _sockfd = sock;
	    _isConnected = true;
	 }
	 else
	 {
	    _lastStatus = tools::funcLastError("connect");
	    ::close(sock);
	 }
      }
      else
      {
	 _lastStatus = tools::funcLastError("socket");
	 _isGood = false;
      }
   }

   if (result) freeaddrinfo(result); 
}


void tcpSocket::close()
{
   int ret = 0;

   if (_sockfd > 0)
      ret = ::close(_sockfd);

   if (ret < 0)
   {
      _lastStatus = tools::funcLastError("close");
      _isGood = false;
   }

   _sockfd = -1;
   _isConnected = false;
}


socketHost tcpSocket::getLocalAddr() 
{
   socketHost ret;
   uint16_t port;
   char ipstr[NET_ADDRLEN];
   struct sockaddr_storage saddr;
   socklen_t saddr_len;
   saddr_len = sizeof(saddr);


   if (isConnected() && getsockname(_sockfd, (struct sockaddr *) &saddr, &saddr_len) != 0)
   {
      _isGood = false;
      _lastStatus = tools::funcLastError("getsockname");  
   }
   else
   {

      if (saddr.ss_family == AF_INET)  // IPV4
      {
	 
	 struct sockaddr_in *saddr4 = (struct sockaddr_in *) &saddr;
	 
	 port = ntohs(saddr4->sin_port);
	 
	 inet_ntop(AF_INET, &saddr4->sin_addr, ipstr, sizeof(ipstr));
      
      } 
      else { // IPV6
	 
	 struct sockaddr_in6 *saddr6 = (struct sockaddr_in6 *) &saddr;
	 
	 port = ntohs(saddr6->sin6_port);
    
	 inet_ntop(AF_INET6, &saddr6->sin6_addr, ipstr, sizeof(ipstr));
      }

      ret.addr = string(ipstr);
      ret.port = port;
   }

   return ret;
}

socketHost tcpSocket::getRemoteAddr()
{
   socketHost ret;
   uint16_t port;
   char ipstr[NET_ADDRLEN];
   struct sockaddr_storage saddr;
   socklen_t saddr_len;
   saddr_len = sizeof(saddr);


   if (isConnected() && getpeername(_sockfd, (struct sockaddr *) &saddr, &saddr_len) != 0)
   {
      _isGood = false;
      _lastStatus = tools::funcLastError("getpeername");  
   }
   else
   {
      if (saddr.ss_family == AF_INET)  // IPV4
      {	 
	 struct sockaddr_in *saddr4 = (struct sockaddr_in *) &saddr;
	 
	 port = ntohs(saddr4->sin_port);
	 
	 inet_ntop(AF_INET, &saddr4->sin_addr, ipstr, sizeof(ipstr));
      } 
      else  // IPV6
      { 
	 struct sockaddr_in6 *saddr6 = (struct sockaddr_in6 *) &saddr;
	 
	 port = ntohs(saddr6->sin6_port);
    
	 inet_ntop(AF_INET6, &saddr6->sin6_addr, ipstr, sizeof(ipstr));
      }

      ret.addr = string(ipstr);
      ret.port = port;
   }

   return ret;
}


void tcpSocket::setRecvLen(const size_t &size)
{
   _recvLength = min(size, (size_t) NET_RECVMAX);
}


void tcpSocket::setSendLen(const size_t &size)
{
   _sendLength = min(size, (size_t) NET_SENDMAX);
}


size_t tcpSocket::sendMsg(tcpMessage *msg)
{
   char *ptr;
   size_t len;
   int sent_total = 0;

   ptr = (char *) msg->payload();
   len = min(msg->length(), _sendLength);

   sent_total = send(_sockfd, ptr, len, 0);
    
   if (sent_total < 0)
   {
      if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)
      {
	 _isGood = false;
	 _lastStatus = tools::funcLastError("send");
      }
   }

   return max(0, sent_total);
}


size_t tcpSocket::recvMsg(tcpMessage *msg)
{
   char *ptr;
   size_t len;
   int recv_total = 0;
   
   ptr = (char *) msg->payload();
   len = min(msg->maxlen(), _recvLength);

   recv_total = recv(_sockfd, ptr, len, 0);

   if (recv_total < 0)
   {
      if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)
      {
	 _lastStatus = tools::funcLastError("recv");
	 _isGood = false;
      }
   }
   else if (recv_total == 0 && len != 0)
   {
      _lastStatus = "Disconnected";
      _isConnected = false;
   }
   
   recv_total = max(0, recv_total);
   msg->setLength(recv_total);
   
   return recv_total;
}


void tcpSocket::connectSocket(const int &fd)
{
   if (_isConnected || _sockfd > 0) close();

   _sockfd = fd;
   _isConnected = true;
}

int tcpSocket::getSocketOpts()
{
   int copts;

   copts = fcntl(_sockfd, F_GETFL);

   if (copts < 0) 
   {
      _isGood = false;
      _lastStatus = tools::funcLastError("fcntl");
   }

   return copts;
}


void tcpSocket::setSocketOpts(const int &opts)
{
   if (fcntl(_sockfd, F_SETFL, opts) < 0) 
   {
      _isGood = false;
      _lastStatus = tools::funcLastError("fcntl");
   }  
}

void tcpSocket::enableBlocking(const bool &val)
{
   int copts = getSocketOpts();

   if (val)
      setSocketOpts(copts & ~O_NONBLOCK);
   else
      setSocketOpts(copts | O_NONBLOCK);
}


void tcpSocket::enableDebug(const bool &val)
{
   int optval = (val)? 1 : 0;

   int ret = setsockopt(_sockfd, SOL_SOCKET, SO_DEBUG, &optval, sizeof(optval));

   if (ret < 0)
   {
      _isGood = false;
      _lastStatus = tools::funcLastError("enableDebug");
   }
}


void tcpSocket::enableLinger(const bool &val, const int &secs)
{
   struct linger optval;

   optval.l_onoff = (val)? 1 : 0;
   optval.l_linger = secs;

   int ret = setsockopt(_sockfd, SOL_SOCKET, SO_LINGER, &optval, sizeof(optval));

   if (ret < 0)
   {
      _isGood = false;
      _lastStatus = tools::funcLastError("enableLinger");
   }
}


void tcpSocket::enableReuseAddr(const bool &val)
{
   int optval = (val)? 1 : 0;

   int ret = setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

   if (ret < 0)
   {
      _isGood = false;
      _lastStatus = tools::funcLastError("enableReuseAddr");
   }
}


void tcpSocket::enableKeepAlive(const bool &val)
{
   int optval = (val)? 1 : 0;

   int ret = setsockopt(_sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval));

   if (ret < 0)
   {
      _isGood = false;
      _lastStatus = tools::funcLastError("enableKeepAlive");
   }
}



