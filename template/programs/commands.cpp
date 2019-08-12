#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char **argv){
  //ifstream fileIn;
  ofstream fileOut;
  string zxc;
  string filename;
  //cin>>filename;
  //cin>>zxc;
  
  //int start=0;
  int end=atoi(argv[2]);
  int energy=atoi(argv[1]);
  //cout<<"Starting no: ";
  //cin>>start;
  //cout<<"Ending no: ";
  //cin>>end;
  //cout<<"Energy: ";
  //cin>>energy;

  string fileO = "commands.txt";
  fileOut.open(fileO);
  for(int i=1; i<=end;i++){
    fileOut<<"qsub runurqmdevgen.qsub "<<energy;
    //fileOut<<"qsub -V -b n -cwd genUrqmd.sh "<<i;
    //fileOut<<"rm -r *."<<i;
    //fileOut<<"qdel "<<i;
    if(i!=end){
      fileOut<<" && ";
    }
  }

 return 0;
}
