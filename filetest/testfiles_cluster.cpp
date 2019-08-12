#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <vector>

using namespace std;

bool exist_file(const string& name); //return 1 if file exist
void prnt_qsublist(FILE* qsub_list, int energy, bool first);   //print list of jobs to do
void prnt_runMClist(FILE* runMClist, int energy, bool first, int job_folder);  //needs go to test folder and run runMC.C
void prnt_recolist(FILE *recolist, int energy, bool first, int job_folder);    //needs go to test folder, copy reco.C and run reco.C
                                                          //in both need source, $1 is number of simulation (i numer), $2 is energy
int n_Events(string inputfile_path); //return numer of events in job
int check_filesize(const string filename,const unsigned long size_min);  //retun 1 if file is smaller for this energy than required, 0 otherwise
void hm_folders(int energy,int &folder_number, vector<string>&job_folder); //how many folders is in Urqmd.energyGeV folder
void prnt_failedjobs(int energy, int i);//print list of folders with errors jobs in Urqmd folder
bool first_list[3]{true,true,true}; //bool helps print qsub reco and runMC list  1st array for qsub 2nd for  runMC 3rd for reco      

//min size of mpddst and evetest boards, 1st array for 4GeV 2nd for 5GeV .... last for 11GeV, first 8 arrays are  for 500 events, second 8 is for 200 events!
unsigned long mpddst_min[16]{660000000,770000000,850000000,1100000000,1200000000,1400000000,1550000000,1700000000,250000000,300000000,330000000,430000000,470000000,550000000,600000000,660000000};
unsigned long evetest_min[16]{3100000000,3600000000,4300000000,5300000000,5600000000,6400000000,6600000000,7300000000,1160000000,1340000000,1610000000,1980000000,2090000000,2390000000,2470000000,2730000000};
 

int main(int argc, char **argv)
{
    if(argc == 3)
    {
        //int max_num_folders = atoi(argv[3]);
        int energy = atoi(argv[2]);

        int num_of_errors[5]{0,0,0,0,0}; //1 array - sum of failed jobs 2 - number of runurqmd.sh fails 3 - nuber of Urqmd fails 4 - of runMC.C 5 - of reco.C

        string GeV_folder = "Urqmd." + to_string(energy) + "GeV";
        string make = "mkdir -p " + GeV_folder;
        string rm = "rm " + GeV_folder + "/jobs.txt";
        string system_command = "ls /eos/nica/mpd/data/" + GeV_folder + ">>" + GeV_folder + "/jobs.txt";
        system(make.c_str());
        system(rm.c_str());
        system(system_command.c_str());

        string listjobs = "Urqmd." + to_string(energy) + "GeV/failed_folders.txt";
        string command = "rm " + listjobs;
        system(command.c_str());

        //string path = "/eos/nica/mpd/data"; //if you have gen and TestEvUrqmd inside Urqmd.energy.Gev folder just change path for your location of Urqmd.energy.Gev folder
        string path = argv[1];
        string Urqmd_folder = path + "/Urqmd." + to_string(energy) + "GeV";

        string error = GeV_folder + "/errors." + to_string(energy) + "GeV.txt";
        string mpddsd_list = GeV_folder + "/urqmd34." + to_string(energy) + "gev.list.txt";
        string error_cat = GeV_folder + "/error_category." + to_string(energy) + "GeV.txt";
        string qsblst = GeV_folder + "/qsublist.txt";
        string runMClst = GeV_folder + "/runMClist.txt";
        string recolst = GeV_folder + "/recolist.txt";
        FILE *file = fopen(error.c_str(),"w+"); //contains what happend and where
        FILE *mpddst = fopen(mpddsd_list.c_str(), "w+"); //contains mpddst.root files
        FILE *error_category = fopen(error_cat.c_str(),"w+"); //contains category of error
        FILE *qsub_list = fopen(qsblst.c_str(),"w+");
        FILE *runMClist = fopen(runMClst.c_str(),"w+");
        FILE *recolist = fopen(recolst.c_str(),"w+");

         ifstream Urqmd(Urqmd_folder);
            if(!Urqmd)
            {
                cout << "Big problems here, maybe you type wrong energy, check errors file!\nExiting...\n";
                fprintf(file,"Folder Urqmd.%dGev not found. runurqmdevgen.qsub has failed\n",energy);
                fclose(file);
                num_of_errors[1]++;
                return 0;      
        }
        
        vector<string>job_folder;
        int folder_number = 0;
        hm_folders(energy,folder_number, job_folder);

        cout << "Checking "<< folder_number << " folders" <<endl;
        int nEvents = 0;
        int last = 0;
        
        for(string i : job_folder)
        {
            string job_folder = Urqmd_folder + "/" + i;
            ifstream job(job_folder);
            if(!job)
            {
                fprintf(file,"Folder %s not found. runurqmdevgen.qsub has failed\n",i.c_str());
                num_of_errors[1]++;
                //prnt_qsublist(qsub_list,energy,first_list[0]);
                //first_list[0] = false;
                continue;
            }

            string gen_folder = job_folder + "/gen";
            ifstream gen(gen_folder);
            if(!gen)
            {
                fprintf(file,"Folder gen in %s not found. runurqmdevgen.qsub has failed\n",i.c_str());
                num_of_errors[1]++;
                //prnt_qsublist(qsub_list,energy,first_list[0]);
                //first_list[0] = false;
                prnt_failedjobs(energy,stoi(i));
                continue;
            }
            //check how much events was in this job, works only if gen folder exist
            string inputfile = gen_folder + "/inputfile";
            last = nEvents;
            if(n_Events(inputfile) > 10000)
            {
                nEvents = last;
            }else
            {
                nEvents = n_Events(inputfile);  
            }
            //cout <<nEvents<<endl; 
            int shift = 0;
            if(nEvents == 200) // shift is nedeed to get correct min_size for set energy and nEvents
            {
                shift = energy + 4;
            }else
            {
                shift = energy - 4;
            }
            
            string ftestf14 = gen_folder + "/test.f14";
            if(exist_file(ftestf14) != 1)
            {
                fprintf(file,"File test.f14 in gen in %s not found. Urqmd has failed\n",i.c_str());
                num_of_errors[2]++;
                //prnt_qsublist(qsub_list,energy,first_list[0]);
                //first_list[0] = false;
                prnt_failedjobs(energy,stoi(i));
                continue;
            }

            string TestEvUrqmd_folder = job_folder + "/TestEvUrqmd";
            ifstream Test_folder(TestEvUrqmd_folder);
            if(!Test_folder)
            {
                fprintf(file,"Folder TestEvUrqmd in %s not found. runurqmdevgen.qsub has failed\n",i.c_str());
                num_of_errors[1]++;
                //prnt_qsublist(qsub_list,energy,first_list[0]);
                //first_list[0] = false;
                prnt_failedjobs(energy,stoi(i));
                continue;
            }

            string fevetest = TestEvUrqmd_folder + "/evetest.root";
            if(exist_file(fevetest) != 1)
            {
                fprintf(file,"File evetest.root in TestEvUrqmd in %s not found. Macro runMC.C has failed\n",i.c_str());
                num_of_errors[3]++;
                prnt_runMClist(runMClist,energy,first_list[1],stoi(i));
                first_list[1] = false;
                prnt_failedjobs(energy,stoi(i));
                continue;
            }
            if (check_filesize(fevetest,evetest_min[shift]))
            {
                fprintf(file,"File evetest.root in TestEvUrqmd in %s is too small. Macro runMC.C has failed\n",i.c_str());
                num_of_errors[3]++;
                prnt_runMClist(runMClist,energy,first_list[1],stoi(i));
                first_list[1] = false;
                prnt_failedjobs(energy,stoi(i));
                continue;
            }
            
            string fmpddst = TestEvUrqmd_folder + "/mpddst.root";
            if(exist_file(fmpddst) != 1)
            {
                fprintf(file,"File mpddst.root in TestEvUrqmd %s not found. Macro reco.C has failed\n",i.c_str());
                num_of_errors[4]++;
                prnt_recolist(recolist,energy,first_list[2],stoi(i));
                first_list[2] = false;
                prnt_failedjobs(energy,stoi(i));
                continue;
            }
            if(check_filesize(fmpddst,mpddst_min[shift]))
            {
                fprintf(file,"File mpddst.root in TestEvUrqmd %s is too small. Macro reco.C has failed\n",i.c_str());
                num_of_errors[4]++;
                prnt_recolist(recolist,energy,first_list[2],stoi(i));
                first_list[2] = false;
                prnt_failedjobs(energy,stoi(i));
                continue;   
            }
            //if everything is okay make list of mpddst.root files
            fprintf(mpddst,"%s\n",fmpddst.c_str());
        }
        cout << "Making qsub list of failed jobs" << endl;
        cout << "Printing list of mpddst.root files" << endl;

        num_of_errors[0] = num_of_errors[1] + num_of_errors[2] + num_of_errors[3] + num_of_errors[4];
        float percentage_failed = (float)num_of_errors[0]/folder_number*100;

        cout << "Printing category of errors list " << endl;

        fprintf(error_category,"%d/%d - %.2f%% of jobs has failed at all\n %d are runurqmd errors\n %d are Urqmd errors\n %d are runMC.C errors\n %d are reco.C errors",num_of_errors[0],folder_number,percentage_failed,num_of_errors[1],num_of_errors[2],num_of_errors[3],num_of_errors[4]);

        fclose(file);
        fclose(mpddst);
        fclose(error_category);
        fclose(qsub_list);
        fclose(runMClist);
        fclose(recolist);
        return 0;
    }else
    {
        fprintf(stderr,"Wrong numer of arguments!\nExiting...\n");
        return 1;
    
    }
    
}

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