/*
    Łukasz Sawicki
    l.sawicki99@gmail.com
*/
#include "testlibs.hpp"

int main(int argc, char **argv)
{
    if(argc == 3)
    {
        //min size of mpddst and evetest boards, 1st array for 4GeV 2nd for 5GeV .... last for 11GeV, first 8 arrays are  for 500 events, second 8 is for 200 events!
        unsigned long mpddst_min[16]{660000000,770000000,850000000,1100000000,1200000000,1400000000,1550000000,1700000000,250000000,300000000,330000000,430000000,470000000,550000000,600000000,660000000};
        unsigned long evetest_min[16]{3100000000,3600000000,4300000000,5300000000,5600000000,6400000000,6600000000,7300000000,1160000000,1340000000,1610000000,1980000000,2090000000,2390000000,2470000000,2730000000};
        bool first_list[3]{true,true,true}; //bool helps print qsub reco and runMC list  1st array for qsub 2nd for  runMC 3rd for reco

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
        //FILE *qsub_list = fopen(qsblst.c_str(),"w+");
        FILE *runMClist = fopen(runMClst.c_str(),"w+");
        FILE *recolist = fopen(recolst.c_str(),"w+");

         ifstream Urqmd(Urqmd_folder);
            if(!Urqmd)
            {
                cout << "Big problems here, maybe you type wrong energy, check errors file!\nExiting...\n";
                fprintf(file,"Folder Urqmd.%dGev not found. run_UrQMD.qsub has failed\n",energy);
                fclose(file);
                num_of_errors[1]++;
                return 0;      
            }
        
        //checking how many folders need to be checked
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
                fprintf(file,"Folder %s not found. run_UrQMD.qsub has failed\n",i.c_str());
                num_of_errors[1]++;
                //prnt_qsublist(qsub_list,energy,first_list[0]);
                //first_list[0] = false;
                continue;
            }

            string gen_folder = job_folder + "/gen";
            ifstream gen(gen_folder);
            if(!gen)
            {
                fprintf(file,"Folder gen in %s not found. run_UrQMD.qsub has failed\n",i.c_str());
                num_of_errors[1]++;
                //prnt_qsublist(qsub_list,energy,first_list[0]);
                //first_list[0] = false;
                prnt_failedjobs(energy,stoi(i));
                continue;
            }
            //check how much events was in this job, works only if gen folder exist
            string inputfile = gen_folder + "/inputfile";
            last = nEvents;
            if(n_Events(inputfile) > 10000) //this number mean error
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
                fprintf(file,"Folder TestEvUrqmd in %s not found. run_UrQMD.qsub has failed\n",i.c_str());
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
        //cout << "Making qsub list of failed jobs" << endl;
        cout << "Printing list of mpddst.root files" << endl;

        num_of_errors[0] = num_of_errors[1] + num_of_errors[2] + num_of_errors[3] + num_of_errors[4];
        float percentage_failed = (float)num_of_errors[0]/folder_number*100;

        cout << "Printing category of errors list " << endl;

        fprintf(error_category,"%d/%d - %.2f%% of jobs has failed at all\n %d are run_UrQMD errors\n %d are Urqmd errors\n %d are runMC.C errors\n %d are reco.C errors",num_of_errors[0],folder_number,percentage_failed,num_of_errors[1],num_of_errors[2],num_of_errors[3],num_of_errors[4]);

        fclose(file);
        fclose(mpddst);
        fclose(error_category);
        //fclose(qsub_list);
        fclose(runMClist);
        fclose(recolist);
        return 0;
    }else
    {
        fprintf(stderr,"Wrong numer of arguments!\nExiting...\n");
        return 1;
    
    }
    
}