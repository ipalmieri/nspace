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

   cout << mserv.lastStatus() << endl;

   tcpSocket *ssock = mserv.accept();

   tcpMessage msg(256);
   string strin;

   //ssock.enableBlocking(false);

   if (!ssock)
   {
      cout << mserv.lastStatus() << endl;
      exit(1);
   }
   
   while (ssock->isConnected())
   {
      ssock->recvMsg(&msg);

      //cout << to_string(msg.length) << ": " << string(msg.payload) << endl;
      cout << msg.writeString();

      getline(cin, strin);
      msg.readString(strin);
      ssock->sendMsg(&msg);

   }
   
   cout << ssock->lastStatus() << " " << mserv.lastStatus() << endl;

   delete ssock;

   return 0;
}
