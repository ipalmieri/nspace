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


   tcpMessage msg;

   msg.payload = "GET / HTTP/1.1\r\nHost:zero.tr\r\n\r\n";
   msg.length = strlen(msg.payload);

   if (!msock.sendMsg(&msg))
      cout << msock.lastStatus() << endl;

   msg.payload = (char *) malloc(sizeof(char)*128);
   msg.length = 127;

   msock.enableBlocking(false);
   msock.setTimeout(20000);
   

   int i = 0;
   while (msock.isGood() && i < 100)
   {
      cout << endl << "##### ITERATION " << to_string(i) << endl;

      msock.recvMsg(&msg);
      msg.payload[msg.length] = '\0';

      //cout << to_string(msg.length) << ": " << string(msg.payload) << endl;
      cout << string(msg.payload);

      msg.length = 127;
   
      i++;
   }

   cout << endl;

   cout << msock.lastStatus() << endl;
   

   return 0;
}
