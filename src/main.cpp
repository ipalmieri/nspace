#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <tools.h>
#include <neural.h>
#include <stdlib.h> 
#include <list>

#define _WIDTH 40.0
#define _HEIGHT 40.0
#define _NX 40
#define _NY 40
#define _RADIUS 1.5
#define _MAXITER 100
#define _INFILE "data/neural/kmap/sorting_arx_n.out"

using namespace std;
using namespace tools;
using namespace neural;

typedef struct
{
   uint target;
   nsignal *params;
   uint pcount;
} istruct;

vector<istruct> loadfile(string filename)
{
   vector<istruct> ret;
   istruct tmpdata;

   csvReader filedata(filename, ",");

   cout << "reading file " << filename << endl;
   
   filedata.openFile();

   while (filedata.isValid())
   {
      vector<string> line = filedata.getLine();
 
      if (line.size() > 0)
      {
	 tmpdata.target = atoi(line[0].c_str());
	 tmpdata.pcount = line.size() - 1;
	 tmpdata.params = (nsignal *) calloc(tmpdata.pcount, sizeof(nsignal));
 
	 for (uint i = 0; i < tmpdata.pcount; i++)
	 {
	    tmpdata.params[i] = atof(line[i+1].c_str());
	 }
      
	 ret.push_back(tmpdata);
      }
   } 
   
   cout << ret.size() << " lines read." << endl;

   return ret;
}

int main()
{

   string infile = _INFILE;
  
   vector<istruct> tdata = loadfile(infile);
   if (tdata.size() <= 0) exit(1);

   /*
   for (uint i = 0; i < tdata.size(); i++)
   {
      cout << tdata[i].target << " ";
      
      for (uint j = 0; j < tdata[i].pcount; j++)
      {
	 cout << fixed << setprecision(6) << setw(9) << tdata[i].params[j] << " "; 
      }
      cout << endl;
   }
   */


   unsigned nx = _NX;
   unsigned ny = _NY;
   unsigned maxiter = _MAXITER;
   double radius = _RADIUS;

   uint pcount = tdata[0].pcount;
   kohonenCube *kmap = kreateMap(pcount, nx, ny, _WIDTH, _HEIGHT);

   netManager *mng = new netManager(kmap);

   cout << "created ANN with " << kmap->neuronCount() << " neurons." << endl;
   
   //mng->printNeurons();
   //mng->printSynapses();

   
   kTrainer *boss = new kTrainer(kmap, radius, (double)maxiter);
   

   //boss->borderMode = true;

   
   unsigned tind;
   for (unsigned i = 0; i < maxiter; i++)
   {
      vector<nsignal> vecinput;
      
      tind = (unsigned) (double)tdata.size()*(double)rand()/RAND_MAX;
     
      //cout << "index: " << tind << endl;

      for (uint j = 0; j < tdata[tind].pcount; j++)
	 vecinput.push_back(tdata[tind].params[j]);

      
      kmap->setInput(vecinput);
      boss->fitCurrentInput();

      cout << "\r" << 100*(i+1)/maxiter << "% complete.";
   }
   cout << endl;

   cout << "ANN trained with " << maxiter << " samples.";
   cout << " Let's run data again over it." << endl;

   
   double qError = 0.0;
   double tError = 0.0;

   //find BMUs   
   map<uint, kneuron *> klist;
   for (uint i = 0; i < tdata.size(); i++)
   {
      vector<nsignal> vecinput;
      
      for (uint j = 0; j < tdata[i].pcount; j++)
	 vecinput.push_back(tdata[i].params[j]);

      kmap->setInput(vecinput);
      kmap->updateAll();
  
      kneuron *knr = kmap->inputBMU();

      klist[i] = knr;
      
      qError += kmap->quantizationError();
      tError += kmap->topographicError(_WIDTH/(_NX-1) + 0.01);
        
   }

   qError /= tdata.size();
   tError /= tdata.size();

   cout << "Errors ---> Q: " << setw(5) << setiosflags(ios::fixed);
   cout << setprecision(4) << qError;
   cout << " T: " << tError << endl;

   
   //find distances
   for (uint i = 0; i < tdata.size(); i++)
   {
      map<uint, double> dlist;
      list<uint> olist;

      //cout << "CN: " << setw(5) << i << " (";
      //cout << tdata[i].target << ") --> ";

      for (uint j = 0; j < tdata.size(); j++)
      {
	 if (i != j)
	 {
	    kneuron *me = klist[i];
	    kneuron *you = klist[j];

	    double dist = neural::kdistance(me->getPosition(), you->getPosition());
	    
	    list<uint>::iterator it = olist.begin();
	    while (it != olist.end() && dlist[(*it)] < dist) ++it;

	    olist.insert(it, j);

	    dlist[j] = dist;	    

	 }
      }
      
      /*
      unsigned ind = 0;
      for (list<uint>::iterator it = olist.begin(); it != olist.end(); ++it)
      {
	 cout << setw(5) << (*it);
	 cout << " ";
	 cout << "(";
	 cout << setw(2) << setprecision(2) << dlist[(*it)];
	 cout << ")  ";
	 cout << tdata[(*it)].target;
	 cout << " ";
	 
	 if (ind == 4) break;
	 ind++;
      }

      kposition pos = klist[i]->getPosition();
      cout << "[ " << setw(2) << setprecision(2) << pos.x << " ";
      cout << setw(2) << setprecision(2) << pos.y << " ";
      cout << setw(2) << setprecision(2) << pos.z << " ]";

      cout << endl;
      */
   }

   

   kMan kprin(kmap);

   kmap->updateAll();
   kmap->updateAll();
   
   kprin.saveUMatrixImage("data/sorting.jpeg", _WIDTH/(_NX-1) + 0.01);
   //kprin.printUMatrix(_WIDTH/(_NX-1) + 0.01);


   //kprin.printKmap();
   //mng->printNeurons();
   //mng->printSynapses();

   
   //print neuron BMUs
   imageMap pic(kmap->xNeurons(), kmap->yNeurons(), 3);
   imageMap pic1(kmap->xNeurons(), kmap->yNeurons(), 3);

   for (uint i = 0; i < pic.linearSize(); i++) {
      pic[i] = (imageMap::pixel) 255;
      pic1[i] = (imageMap::pixel) 255; 
   }

   //uint posx = 0;
   //uint posy = 0;
   //uint idx = pic.width()*pic.channels()* + posx*pic.channels();
   //pic[idx] = (imageMap::pixel) 0;
   //pic[idx] = (imageMap::pixel) 0;
   //pic[idx] = (imageMap::pixel) 0;


   uint colorlst[4][3] = {{0, 0, 0},
			   {255, 0, 0},
			   {0, 255, 0},
			   {0, 0, 255}};
      

   for (uint i = 0; i < tdata.size(); i++)
   {
      kneuron *nr = klist[i];
      
      double posxd = (nr->positionX() + kmap->width()*0.5)/kmap->width();
      double posyd = (nr->positionY() + kmap->height()*0.5)/kmap->height();
      uint posx = (uint) ((double) (pic.width() - 1)*posxd);
      uint posy = (uint) ((double) (pic.height() - 1)*(1.0 - posyd));
      
      uint idx = (unsigned) (pic.width()*pic.channels()*posy + posx*pic.channels());

      //cout << idx << " -> " << posx << "," << posy << " ---  ";
      //cout << pic.width() << " " << pic.height() << " ";
      //cout << nr->positionX() << " " << nr->positionY() << endl;

      uint color_idx = tdata[i].target % 4;

      if (tdata[i].target != 0) {
	 pic1[idx+0] = (imageMap::pixel) colorlst[color_idx][0];
	 pic1[idx+1] = (imageMap::pixel) colorlst[color_idx][1];
	 pic1[idx+2] = (imageMap::pixel) colorlst[color_idx][2]; 
      }
      
      pic[idx+0] = (imageMap::pixel) colorlst[color_idx][0];
      pic[idx+1] = (imageMap::pixel) colorlst[color_idx][1];
      pic[idx+2] = (imageMap::pixel) colorlst[color_idx][2];

   }
   
   //cout << pic.width() << " " << pic.height() << " " << pic.channels() << endl;

   writeJPEG("data/sorting_neurons.jpeg", pic);
   writeJPEG("data/sorting_targets.jpeg", pic1);

   delete boss;
   delete mng;
   delete kmap;



   return 0;
}
