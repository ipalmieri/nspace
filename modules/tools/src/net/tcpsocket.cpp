#include "net/tcpsocket.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "aux.h"

#include <iostream>

using namespace std;
using namespace tools;


tcpSocket::tcpSocket()
{
   _isGood = true;
   _isConnected = false;

   _enableBlocking = true;
   _enableDebug = false;
   _enableLinger = false;
   _enableReuseAddr = false;
   _enableKeepAlive = false;
   
   _sockfd = -1;

   setRecvLen(NET_RECVLEN);
   setSendLen(NET_SENDLEN);
}


tcpSocket::~tcpSocket()
{
   close();
}  


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


void tcpSocket::setRecvLen(const size_t &size)
{
   _recvLength = min((int) size, NET_RECVMAX);

   //_recvBuffer = (char *) realloc(_recvBuffer, _recvLength); 
}


void tcpSocket::setSendLen(const size_t &size)
{
   _sendLength = min((int) size, NET_SENDMAX);

   //_sendBuffer = (char *) realloc(_sendBuffer, _sendLength); 
}


bool tcpSocket::sendMsg(const tcpMessage *msg)
{
   char *prt = NULL;

   size_t len;
   size_t sent_total = 0;
   size_t sent_now = 0;

   prt = msg->payload;

   while (sent_total != msg->length && _isGood)
   {  
      len = min(msg->length - sent_total, _sendLength);

      // select c timeout

      sent_now = send(_sockfd, &(prt[sent_total]), len, 0);

      if (sent_now == -1)
      {
	 _lastStatus = tools::funcLastError("send");

	 _isGood = false;
      }

      sent_total += sent_now;
   }

   return (sent_total == msg->length);
}


bool tcpSocket::recvMsg(const tcpMessage *msg)
{
   
}


