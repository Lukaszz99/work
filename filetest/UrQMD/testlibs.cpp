#include "testlibs.hpp"

bool exist_file(const string& name)
    {
        struct stat buffer;
        return(stat(name.c_str(), &buffer) == 0);
    }

void prnt_qsublist(FILE* qsub_list, int energy, bool first)
    {
        if(first == true){
            fprintf(qsub_list,"qsub runurqmdevgen.qsub %i",energy);
        }else
        {
            fprintf(qsub_list,"&& qsub runurqmdevgen.qsub %d ",energy);
        }      
    }


void prnt_runMClist(FILE* runMClist, int energy, bool first, int job_folder)
{ 
    if(first == true){
        fprintf(runMClist,"qsub runMC_list.qsub %d %d ",job_folder, energy);
    }else
    {
        fprintf(runMClist,"&& qsub runMC_list.qsub %d %d ",job_folder, energy);
    }
}

void prnt_recolist(FILE* recolist, int energy, bool first, int job_folder)
{ 
    if(first == true){
        fprintf(recolist,"qsub reco_list.qsub %d %d ",job_folder, energy);
    }else
    {
        fprintf(recolist,"&& qsub reco_list.qsub %d %d ",job_folder, energy);
    }
}

int n_Events(string inputfile_path)
{
    int nEvents;
    string nev = "nev"; //after world nev in inputfile is number of events
    string tmp;
    fstream inputfile; 
    inputfile.open(inputfile_path.c_str(), ios::in);

    while(inputfile >> tmp)
    {   
        if(nev == tmp)
        {
            string number;
            inputfile >> number;
            nEvents = stoi(number);
            break;
        }
    }

    inputfile.close();
    return nEvents;
}
int check_filesize(const string filename, unsigned long size_min)
{
    struct stat size;
    stat(filename.c_str(),&size);
    if(size.st_size < size_min) //if less than required
    {
        return 1;
    }else
    {   
        return 0;
    }
}
void hm_folders(int energy,int &folder_number, vector<string>&job_folder)
{
    fstream folder_list;
    string listjobs = "Urqmd." + to_string(energy) + "GeV/jobs.txt";
    folder_list.open(listjobs.c_str(), ios::in | ios::out);
    string tmp;
    
    while(folder_list >> tmp )
    {
        if(tmp.compare("jobs.txt") != 0);
            {
                job_folder.push_back(tmp);
                folder_number++;
            }
    }
    folder_list.close();
}
void prnt_failedjobs(int energy, int i)
{
    string listjobs = "Urqmd." + to_string(energy) + "GeV/failed_folders.txt";
    FILE *folder_list = fopen(listjobs.c_str(),"a+");

    fprintf(folder_list,"rm -rf %d\n",i);
    //fprintf(folder_list,"mv %d ../TRASH/\n",i);

    fclose(folder_list);
}