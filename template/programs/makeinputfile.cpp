#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
  int energy = 9;
  int imp = 3;
  int nev = 500;
  if(argc > 3){
    energy = atoi(argv[1]);
    imp = atoi(argv[2]);
    nev = atoi(argv[3]);
  }else if(argc >2){
    energy = atoi(argv[1]);
    imp = atoi(argv[2]);
  }else if(argc > 1){
    energy = atoi(argv[1]);
  }
    

    FILE *inputfile = fopen("inputfile","w+");

    fprintf(inputfile,"pro 209 83\ntar 209 83\n\nnev %d\nimp %d.\n\necm %d.\ntim 200 200\n\neos 0\n\n\n\n\nf13\n#f14\nf15\nf16\nf19\nf20\n\nxxx",nev,imp,energy);
    fclose(inputfile);

    return 0;
}
