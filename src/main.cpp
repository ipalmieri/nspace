#include <iostream>
#include <fstream>
#include <math.h>
#include <tools.h>
#include <stdlib.h> 
#include <list>
#include <string.h>

#include "tools.h"

using namespace std;
using namespace tools;

int main()
{
   
   tcpSocket msock;

   msock.connect("zero.tr", 80);

   socketHost sl = msock.getLocalAddr();
   socketHost sr = msock.getRemoteAddr();

   cout << "Local: " << sl.addr << " " << to_string(sl.port) << endl;
   cout << "Remote: " << sr.addr << " " << to_string(sr.port) << endl;


   tcpMessage msg(256);

   msg.readString("GET / HTTP/1.1\r\nBHost:zero.tr\r\n\r\n");


   if (msock.sendMsg(&msg) != msg.length())
      cout << msock.lastStatus() << endl;

   

   //msg = tcpMessage(1);
   msock.enableBlocking(false);
   

   int i = 0;
   while (msock.isConnected() && i < 100)
   {
      cout << endl << "##### ITERATION " << to_string(i) << endl;

      msock.recvMsg(&msg);

      //cout << to_string(msg.length) << ": " << string(msg.payload) << endl;
      cout << string(msg.payload(), msg.length());
   
      //char c = string(msg.payload(), msg.length())[0];   printf("\nc: %d\n", c);

      i++;
   }

   cout << endl;

   cout << msock.lastStatus() << endl;

   return 0;
}
