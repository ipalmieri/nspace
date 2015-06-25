#include <iostream>
#include <fstream>
#include <math.h>
#include <tools.h>
#include <stdlib.h> 
#include <list>

#include "tools.h"

using namespace std;
using namespace tools;

int main()
{
   tcpSocket msock;

   msock.connect("www.uol.com.br", 80);

   socketHost sl = msock.getLocalAddr();
   socketHost sr = msock.getRemoteAddr();

   cout << "Local: " << sl.addr << " " << to_string(sl.port) << endl;
   cout << "Remote: " << sr.addr << " " << to_string(sr.port) << endl;
  
  

   return 0;
}
