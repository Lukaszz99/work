#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
    int energy = atoi(argv[1]);

    FILE *inputfile = fopen("inputfile","w+");

    fprintf(inputfile,"pro 209 83\ntar 209 83\n\nnev 200\nimp 5.\n\necm %d.\ntim 200 200\n\neos 0\n\n\n\n\nf13\n#f14\nf15\nf16\nf19\nf20\n\nxxx",energy);
    fclose(inputfile);

    return 0;
}
