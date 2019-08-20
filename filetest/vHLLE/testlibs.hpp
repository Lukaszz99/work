#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <vector>

using namespace std;

bool exist_file(const string& name); //return 1 if file exist
void prnt_runMClist(FILE* runMClist, string energy, bool first, int job_folder);  //needs go to test folder and run runMC.C
void prnt_recolist(FILE *recolist, string energy, bool first, int job_folder);    //needs go to test folder, copy reco.C and run reco.C
                                                          //in both need source, $1 is number of simulation (i numer), $2 is energy
int n_Events(string inputfile_path); //return numer of events in job
int check_filesize(const string filename,const unsigned long size_min);  //retun 1 if file is smaller for this energy than required, 0 otherwise
void hm_folders(string energy,int &folder_number, vector<string>&job_folder); //how many folders is in Urqmd.energyGeV folder
void prnt_failedjobs(string energy, int i);//print list of folders with errors jobs in Urqmd folder