#include "TTree.h"
#include "TFile.h"

int main(int argc, char** argv){
  int nev; //number of events that should be in the mpddst.root file

  if(argc !=2) nev = 500;
  else nev = atoi(argv[1]);

  TFile* file = new TFile("mpddst.root");
  TTree* tree = (TTree*)(gDirectory->Get("mpdsim")->Clone());

  if (tree->GetEntries() == nev) return 0;
  else return -1;
}
