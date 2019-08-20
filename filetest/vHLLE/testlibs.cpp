#include "testlibs.hpp"

bool exist_file(const string& name)
    {
        struct stat buffer;
        return(stat(name.c_str(), &buffer) == 0);
    }

void prnt_runMClist(FILE* runMClist, string energy, bool first, int job_folder)
{ 
    if(first == true){
        fprintf(runMClist,"qsub runMC_list.qsub %d %s ",job_folder, energy.c_str());
    }else
    {
        fprintf(runMClist,"&& qsub runMC_list.qsub %d %s ",job_folder, energy.c_str());
    }
}

void prnt_recolist(FILE* recolist, string energy, bool first, int job_folder)
{ 
    if(first == true){
        fprintf(recolist,"qsub reco_list.qsub %d %s ",job_folder, energy.c_str());
    }else
    {
        fprintf(recolist,"&& qsub reco_list.qsub %d %s ",job_folder, energy.c_str());
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
void hm_folders(string energy,int &folder_number, vector<string>&job_folder)
{
    fstream folder_list;
    string listjobs = "vHLLE." + energy + "GeV/jobs.txt";
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
void prnt_failedjobs(string energy, int i)
{
    string listjobs = "vHLLE." + energy + "GeV/failed_folders.txt";
    FILE *folder_list = fopen(listjobs.c_str(),"a+");

    fprintf(folder_list,"%d\n",i);
    //fprintf(folder_list,"rm -rf %d\n",i);
    //fprintf(folder_list,"mv %d ../TRASH/\n",i);

    fclose(folder_list);
}