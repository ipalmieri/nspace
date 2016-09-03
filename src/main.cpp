#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <tools.h>
#include <neural.h>
#include <stdlib.h>
#include <list>

#define _LOGFILE "/tmp/engine.log"
#define _PARFILE "/data/params.conf"

using namespace std;
using namespace tools;
using namespace neural;

/////////////////////////////////////////////////////////////////
// Data structures
/////////////////////////////////////////////////////////////////

typedef struct {
  tools::Real swidth;
  tools::Real sheight;
  uint nx;
  uint ny;
  float radius;
  uint maxiter;
  bool border;
  string datafile;
  string ansfile;
  string outfile;
  string bmuimg;
  string ptsimg;
} parstruct;

typedef struct {
  parstruct* params;
  seriesArray* data;
  vector<std::string>* answer;
  kohonenCube* som;
  netManager* mng;
  map<uint, kneuron*>* klist;
  std::string name;
} simulation;


/////////////////////////////////////////////////////////////////
// Helper functions
/////////////////////////////////////////////////////////////////
void printArray(const seriesArray& arr)
{
  for (uint i = 0; i < arr.sampleCount(); i++) {

    for (uint j = 0; j < arr[i].size(); j++) {

      cout << arr[i][j] << " ";
    }

    cout << endl;
  }
}

/////////////////////////////////////////////////////////////////
// Load simulation parameters to structure
/////////////////////////////////////////////////////////////////
void loadParams(simulation* context)
{
  parstruct* params = new parstruct;

  cout << "Loading parameters..." << endl;

  std::ifstream infile(_PARFILE);


  infile >> params->swidth;
  infile >> params->sheight;
  infile >> params->nx;
  infile >> params->ny;
  infile >> params->radius;
  infile >> params->maxiter;

  uint bord = 0;
  infile >> bord;
  params->border = (bord > 0);

  infile >> params->datafile;
  infile >> params->ansfile;
  infile >> params->outfile;
  infile >> params->bmuimg;
  infile >> params->ptsimg;

  /*
  cout << params->swidth << endl;
  cout << params->sheight << endl;
  cout << params->nx << endl;
  cout << params->ny << endl;
  cout << params->radius << endl;
  cout << params->maxiter << endl;
  cout << params->border << endl;

  cout << params->datafile << endl;
  cout << params->ansfile << endl;
  cout << params->outfile << endl;
  cout << params->bmuimg << endl;
  cout << params->ptsimg << endl;
  */

  context->params = params;
}


/////////////////////////////////////////////////////////////////
// Read data input and create SOM
/////////////////////////////////////////////////////////////////
void createANN(simulation* context)
{
  parstruct* params = context->params;

  cout << "Loading input files..." << endl;

  seriesArray* tdata = new seriesArray(loadSignalCSV<signalSample>
                                       (params->datafile, ','));
  vector< vector<string> > csvans = readCSV(params->ansfile, ",");
  vector<string>* tanswer = new vector<string>;

  for (uint i = 0; i < csvans.size(); i++) {
    tanswer->push_back(csvans[i][0]);
  }


  cout << "Creating ANN..." << endl;

  //printArray(par_arr);
  uint pcount = tdata->signalCount();

  kohonenCube* kmap = kreateMap(pcount, params->nx, params->ny, params->swidth,
                                params->sheight);
  netManager* mng = new netManager(kmap);

  //mng->printNeurons();
  //mng->printSynapses();

  context->data = tdata;
  context->answer = tanswer;
  context->som = kmap;
  context->mng = mng;

  cout << "Created ANN with " << kmap->neuronCount() << " neurons." << endl;
}

/////////////////////////////////////////////////////////////////
// Train SOM
/////////////////////////////////////////////////////////////////
void trainANN(simulation* context)
{
  parstruct* params = context->params;
  seriesArray* tdata = context->data;
  kohonenCube* kmap = context->som;

  cout << "Training ANN.." << endl;

  kTrainer* boss = new kTrainer(kmap, params->radius, (double)params->maxiter);
  boss->borderMode = params->border;

  unsigned tind;
  for (unsigned i = 0; i < params->maxiter; i++) {
    tind = (unsigned) (double)tdata->sampleCount()*(double)rand()/RAND_MAX;

    kmap->setInput((*tdata)[tind]);
    boss->fitCurrentInput();

    cout << "\r" << 100*(i + 1)/params->maxiter << "% complete.";
  }
  cout << endl;

  cout << "ANN trained with " << params->maxiter << " samples." << endl;

  delete boss;
}

/////////////////////////////////////////////////////////////////
// Match points in SOM
/////////////////////////////////////////////////////////////////
void matchData(simulation* context)
{
  kohonenCube* kmap = context->som;
  seriesArray* tdata = context->data;
  parstruct* params = context->params;
  map<uint, kneuron*>* klist = new map<uint, kneuron*>;
  context->klist = klist;

  double qError = 0.0;
  double tError = 0.0;

  cout << "Finding BMUs and calculating errors..." << endl;

  //find BMUs
  for (uint i = 0; i < tdata->sampleCount(); i++) {
    kmap->setInput((*tdata)[i]);
    kmap->updateAll();

    kneuron* knr = kmap->inputBMU();

    (*klist)[i] = knr;

    qError += kmap->quantizationError();
    tError += kmap->topographicError(params->swidth / (params->nx - 1) + 0.01);

    cout << "\r" << 100*(i + 1)/tdata->sampleCount() << "% complete.";

  }
  cout << endl;

  qError /= tdata->sampleCount();
  tError /= tdata->sampleCount();

  cout << "Errors ---> Q: " << setw(5) << setiosflags(ios::fixed);
  cout << setprecision(4) << qError;
  cout << " T: " << tError << endl;

}

/////////////////////////////////////////////////////////////////
// Print results and save output
/////////////////////////////////////////////////////////////////
void printResults(simulation* context)
{
  kohonenCube* kmap = context->som;
  parstruct* params = context->params;
  seriesArray* tdata = context->data;
  vector<string>* tanswer = context->answer;
  map<uint, kneuron*>* klist = context->klist;

  cout << "Saving results..." << endl;

  kMan kprin(kmap);

  kmap->updateAll();
  kmap->updateAll();

  kprin.saveUMatrixImage(params->bmuimg,
                         (double) params->swidth /
                         (double) (params->nx-1) + 0.01);
  
  //kprin.printUMatrix(_WIDTH/(_NX-1) + 0.01);


  //kprin.printKmap();
  //mng->printNeurons();
  //mng->printSynapses();


  //print neuron BMUs
  imageMap pic(kmap->xNeurons(), kmap->yNeurons(), 3);

  for (uint i = 0; i < pic.linearSize(); i++) {
    pic[i] = (imageMap::pixel) 255;
  }

  //uint posx = 0;
  //uint posy = 0;
  //uint idx = pic.width()*pic.channels()* + posx*pic.channels();
  //pic[idx] = (imageMap::pixel) 0;
  //pic[idx] = (imageMap::pixel) 0;
  //pic[idx] = (imageMap::pixel) 0;


  uint color_table[4][3] = { {0, 0, 255},
                             {0, 255, 0},
                             {0, 0, 0},
                             {255, 0, 0} };
  

  csvWriter outcsv(params->outfile, ',');

  for (uint i = 0; i < tdata->sampleCount(); i++) {
    kneuron* nr = (*klist)[i];

    uint nrid = stoi((*tanswer)[i]);
    uint* color = color_table[nrid - 1];

    double posxd = (nr->positionX() + kmap->width()*0.5)/kmap->width();
    double posyd = (nr->positionY() + kmap->height()*0.5)/kmap->height();
    uint posx = (uint) ((double) (pic.width() - 1)*posxd);
    uint posy = (uint) ((double) (pic.height() - 1)*(1.0 - posyd));

    uint idx = (unsigned) (pic.width()*pic.channels()*posy + posx*pic.channels());

    //cout << idx << " -> " << posx << "," << posy << " ---  ";
    //cout << pic.width() << " " << pic.height() << " ";
    //cout << nr->positionX() << " " << nr->positionY() << endl;
    //cout << "color: " << color[0] << " " << color[1] << " " << color[2] << endl;

    pic[idx+0] = (imageMap::pixel) color[0];
    pic[idx+1] = (imageMap::pixel) color[1];
    pic[idx+2] = (imageMap::pixel) color[2];


    vector<string> strpos;
    std::ostringstream sx, sy;

    sx << nr->positionX();
    sy << nr->positionY();

    strpos.push_back(sx.str());
    strpos.push_back(sy.str());

    outcsv.writeLine(strpos);

  }

  //cout << pic.width() << " " << pic.height() << " " << pic.channels() << endl;

  writeJPEG(params->ptsimg.c_str(), pic);

}

/////////////////////////////////////////////////////////////////
// Cleanup function
/////////////////////////////////////////////////////////////////
void cleanup(simulation* context)
{
  if (context->params) {
    delete context->params;
  }
  if (context->data) {
    delete context->data;
  }
  if (context->answer) {
    delete context->answer;
  }
  if (context->som) {
    delete context->som;
  }
  if (context->mng) {
    delete context->mng;
  }
  if (context->klist) {
    delete context->klist;
  }

  context->params = NULL;
  context->data = NULL;
  context->answer = NULL;
  context->som = NULL;
  context->mng = NULL;
  context->klist = NULL;
}


/////////////////////////////////////////////////////////////////
// Simulation process
/////////////////////////////////////////////////////////////////
void simulate()
{
  simulation context;

  loadParams(&context);

  createANN(&context);

  trainANN(&context);

  matchData(&context);

  printResults(&context);

  cleanup(&context);
}

/////////////////////////////////////////////////////////////////
// Main func
/////////////////////////////////////////////////////////////////
int main()
{

  logger::startLog(_LOGFILE);

  simulate();

  return 0;
}
