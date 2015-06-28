#include "net/tcpsocket.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "aux.h"

#include <iostream>

using namespace std;
using namespace tools;


tcpSocket::tcpSocket()
{
   _isGood = true;
   _isConnected = false;
   
   _sockfd = -1;

   setRecvLen(NET_RECVLEN);
   setSendLen(NET_SENDLEN);

   _socketTimeout = 0;
}


tcpSocket::~tcpSocket()
{
   close();
}  

// fix this: should do non_blockin connect, with timeout
void tcpSocket::connect(const std::string &hostname, const uint16_t &port)
{
   if (isConnected()) return;

   struct addrinfo *result, *rp;
   struct addrinfo hints;
   int sock, ret;

   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_family = AF_UNSPEC;
   hints.ai_flags = (AI_V4MAPPED|AI_ADDRCONFIG);
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_protocol = 0;    

   ret = getaddrinfo(hostname.c_str(), std::to_string(port).c_str(), &hints, &result);

   if (ret != 0) 
   {
      _isGood = false;
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
	    _isGood = true;
	 }
	 else
	 {
	    _lastStatus = tools::funcLastError("connect");
	 }
      }
      else
      {
	 _lastStatus = tools::funcLastError("socket");
      }
   }

   if (result) freeaddrinfo(result); 
}


void tcpSocket::close()
{
   int ret = 0;

   if (isGood())
      ret = ::close(_sockfd);

   if (ret < 0)
   {
      _lastStatus = tools::funcLastError("close");

      _isGood = false;

   }

   _sockfd = -1;
   _isConnected = false;
}


bool tcpSocket::isConnected()
{
   //fix this: should test sockfd periodically

   _isConnected = _isGood && _isConnected;

   return  _isConnected;
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


bool tcpSocket::sendMsg(tcpMessage *msg)
{
   char *prt = NULL;
   size_t len;
   size_t sent_total = 0;
   ssize_t sent_now = 0;
   fd_set writeset;
   struct timeval tout;
   int retval;

   prt = msg->payload;

   while (sent_total != msg->length && _isGood)
   {  
      len = min(msg->length - sent_total, _sendLength);

      FD_ZERO(&writeset);
      FD_SET(_sockfd, &writeset);
      
      tout.tv_sec = _socketTimeout / 1000;
      tout.tv_usec = _socketTimeout % 1000;

      retval = select(_sockfd + 1, NULL, &writeset, NULL, &tout);

      if (retval == -1)
      {
	 _lastStatus = tools::funcLastError("select");

	 _isGood = false;

	 return false;
      }
      else if (retval == 0)
      {
	 _lastStatus = "select(): write timeout";

	 return false;
      }
      else
      {

	 sent_now = send(_sockfd, &(prt[sent_total]), len, 0);
    
	 if (sent_now < 0)
	 {
	    if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)
	    {
	       _lastStatus = tools::funcLastError("send");

	       _isGood = false;
	    }
	 }
	 else
	 {
	    sent_total += sent_now;
	 }
      }
   }
      
    
   return (sent_total == msg->length);
}


bool tcpSocket::recvMsg(tcpMessage *msg)
{  
   char *prt = NULL;
   ssize_t len;
   ssize_t recv_total = 0;
   fd_set readset;
   struct timeval tout;
   int retval;

   if (msg->length <= 0) 
   {
      _lastStatus = "recvMsg(): zero-length msg";
      return false;
   }


   prt = msg->payload;
   len = msg->length;
   msg->length = 0;

   FD_ZERO(&readset);
   FD_SET(_sockfd, &readset);
      
   tout.tv_sec = _socketTimeout / 1000;
   tout.tv_usec = _socketTimeout % 1000;

   retval = select(_sockfd + 1, &readset, NULL, NULL, &tout);

   if (retval == -1)
   {
	 if (errno != EINTR)
	 {
	    _lastStatus = tools::funcLastError("select");

	    _isGood = false;
	 }

	 return false;      
   }
   else if (retval == 0 && _socketTimeout != 0)
   {
      _lastStatus = "select(): read timeout";
      _isGood = false;

      return false;
   }
   else
   {
      recv_total = recv(_sockfd, prt, len, 0);

      if (recv_total < 0)
      {

	 _lastStatus = tools::funcLastError("recv");
	 
	 if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)
	    _isGood = false;

      }
      else
      {
	 msg->length = recv_total;
      }
   }
    
   return (recv_total > 0);
}


int tcpSocket::getSocketOpts()
{
   int copts;

   copts = fcntl(_sockfd, F_GETFL);

   if (copts < 0) {

      _isGood = false;

      _lastStatus = tools::funcLastError("fcntl");
   }

   return copts;
}


void tcpSocket::setSocketOpts(const int &opts)
{
   if (fcntl(_sockfd, F_SETFL, opts) < 0) {

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

   setsockopt(_sockfd, SOL_SOCKET, SO_DEBUG, &optval, sizeof(optval));
}


void tcpSocket::enableLinger(const bool &val)
{
   struct linger optval;

   optval.l_onoff = (val)? 1 : 0;
   optval.l_linger = _socketTimeout;

   setsockopt(_sockfd, SOL_SOCKET, SO_LINGER, &optval, sizeof(optval));
}


void tcpSocket::enableReuseAddr(const bool &val)
{
   int optval = (val)? 1 : 0;

   setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}


void tcpSocket::enableKeepAlive(const bool &val)
{
   int optval = (val)? 1 : 0;

   setsockopt(_sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval));
}


