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
   
   tcpServer mserv(9999);

   tcpSocket *ssock = mserv.accept();

   tcpMessage msg;
   msg.payload = (char *) malloc(sizeof(char)*128);
   msg.length = 127;

   //ssock.enableBlocking(false);
   //ssock.setTimeout(20000);


   if (!ssock)
   {
      cout << mserv.lastStatus() << endl;
      exit(1);
   }
   
   while (ssock->isConnected())
   {
      ssock->recvMsg(&msg);
      msg.payload[msg.length] = '\0';

      //cout << to_string(msg.length) << ": " << string(msg.payload) << endl;
      cout << string(msg.payload);

      msg.length = 127;      
   }
   
   cout << mserv.lastStatus() << endl;

   delete ssock;

   return 0;
}
