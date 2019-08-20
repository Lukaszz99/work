/*
    Łukasz Sawicki
    l.sawicki99@gmail.com
*/
#include "testlibs.hpp"

int main(int argc, char **argv)
{
    if(argc == 3)
    {

        //int max_num_folders = atoi(argv[3]);
        string energy = argv[2];

        int num_of_errors[5]{0,0,0,0,0}; //1 array - sum of failed jobs 2 - number of run_vHLLE.qsub fails 3 - nuber of vHLLE.C fails 4 - of runMC.C 5 - of reco.C
        //bool first_list[2]{true,true}; //bool helps print reco and runMC list 1st for runMC 2nd for reco

        string GeV_folder = "vHLLE." + energy + "GeV";
        string make = "mkdir -p " + GeV_folder;
        string rm = "rm " + GeV_folder + "/jobs.txt"; // remove, because we dont want to overwrite txt folder
        string system_command = "ls /eos/nica/mpd/data/" + GeV_folder + ">>" + GeV_folder + "/jobs.txt";
        system(make.c_str());
        system(rm.c_str());
        system(system_command.c_str());

        string listjobs = "vHLLE." + energy + "GeV/failed_folders.txt";
        string command = "rm " + listjobs;
        system(command.c_str());

        //string path = "/eos/nica/mpd/data"; //if you have gen and TestEvUrqmd inside Urqmd.energy.Gev folder just change path for your location of Urqmd.energy.Gev folder
        string path = argv[1];
        string vHLLE_folder = path + "/vHLLE." + energy + "GeV";

        string error = GeV_folder + "/errors." + energy + "GeV.txt";
        string mpddsd_list = GeV_folder + "/vHLLE." + energy + "gev.list.txt";
        string error_cat = GeV_folder + "/error_category." + energy + "GeV.txt";
        //string qsblst = GeV_folder + "/qsublist.txt";
        //string runMClst = GeV_folder + "/runMClist.txt";
        //string recolst = GeV_folder + "/recolist.txt";
        FILE *file = fopen(error.c_str(),"w+"); //contains what happend and where
        FILE *mpddst = fopen(mpddsd_list.c_str(), "w+"); //contains mpddst.root files
        FILE *error_category = fopen(error_cat.c_str(),"w+"); //contains category of error
        //FILE *runMClist = fopen(runMClst.c_str(),"w+");
        //FILE *recolist = fopen(recolst.c_str(),"w+");

         ifstream vHLLE(vHLLE_folder);
            if(!vHLLE)
            {
                cout << "Big problems here, maybe you type wrong energy, check errors file!\nExiting...\n";
                fprintf(file,"Folder vHLLE.%sGev not found. run_vHLLE.qsub has failed\n",energy);
                fclose(file);
                num_of_errors[1]++;
                return 0;      
            }
        
        //checking how many folders need to be checked
        vector<string>job_folder;
        int folder_number = 0;
        hm_folders(energy,folder_number, job_folder);

        cout << "Checking "<< folder_number << " folders" <<endl;
        for(string i : job_folder)
        {
            string job_folder = vHLLE_folder + "/" + i;
            ifstream job(job_folder);
            if(!job)
            {
                fprintf(file,"Folder %s not found. run_vHLLE.qsub has failed\n",i.c_str());
                num_of_errors[1]++;
                continue;
            }

            string gen_folder = job_folder + "/gen";
            ifstream gen(gen_folder);
            if(!gen)
            {
                fprintf(file,"Folder gen in %s not found. run_vHLLE.qsub has failed\n",i.c_str());
                num_of_errors[1]++;
                prnt_failedjobs(energy,stoi(i));
                continue;
            }
            string output_folder = job_folder + "/output";
            ifstream output(output_folder);
            if(!output)
            {
                fprintf(file,"Folder output in %s not found. run_vHLLE.qsub has failed\n",i.c_str());
                num_of_errors[1]++;
                prnt_failedjobs(energy,stoi(i));
                continue;
            }
            
            string vHLLE_sim = output_folder + "/vHLLE_sim.root";
            if(exist_file(vHLLE_sim) != 1)
            {
                fprintf(file,"File vHLLE_sim.root in output in %s not found. Bash script produced by vHLLE macro has failed\n",i.c_str());
                num_of_errors[2]++;
                prnt_failedjobs(energy,stoi(i));
                continue;
            }

            string fevetest = output_folder + "/evetest.root";
            if(exist_file(fevetest) != 1)
            {
                fprintf(file,"File evetest.root in output in %s not found. Macro runMC.C has failed\n",i.c_str());
                num_of_errors[3]++;
                //prnt_runMClist(runMClist,energy,first_list[1],stoi(i));
                //first_list[0] = false;
                prnt_failedjobs(energy,stoi(i));
                continue;
            }
           /*if (check_filesize(fevetest,evetest_min[???])) what size is correct?
            {
                fprintf(file,"File evetest.root in output in %s is too small. Macro runMC.C has failed\n",i.c_str());
                num_of_errors[3]++;
                prnt_runMClist(runMClist,energy,first_list[1],stoi(i));
                first_list[0] = false;
                prnt_failedjobs(energy,stoi(i));
                continue;
            }
            */
            
            string fmpddst = output_folder + "/mpddst.root";
            if(exist_file(fmpddst) != 1)
            {
                fprintf(file,"File mpddst.root in output %s not found. Macro reco.C has failed\n",i.c_str());
                num_of_errors[4]++;
                //prnt_recolist(recolist,energy,first_list[2],stoi(i));
                //first_list[2] = false;
                prnt_failedjobs(energy,stoi(i));
                continue;
            }
            /*if(check_filesize(fmpddst,mpddst_min[???]))
            {
                fprintf(file,"File mpddst.root in output %s is too small. Macro reco.C has failed\n",i.c_str());
                num_of_errors[4]++;
                prnt_recolist(recolist,energy,first_list[2],stoi(i));
                first_list[2] = false;
                prnt_failedjobs(energy,stoi(i));
                continue;   
            }
            */
            //if everything is okay make list of mpddst.root files
            fprintf(mpddst,"%s\n",fmpddst.c_str());
        }
        //cout << "Making qsub list of failed jobs" << endl;
        cout << "Printing list of mpddst.root files" << endl;

        num_of_errors[0] = num_of_errors[1] + num_of_errors[2] + num_of_errors[3] + num_of_errors[4];
        float percentage_failed = (float)num_of_errors[0]/folder_number*100;

        cout << "Printing category of errors list " << endl;

        fprintf(error_category,"%d/%d - %.2f%% of jobs has failed at all\n %d are runurqmd errors\n %d are Urqmd errors\n %d are runMC.C errors\n %d are reco.C errors",num_of_errors[0],folder_number,percentage_failed,num_of_errors[1],num_of_errors[2],num_of_errors[3],num_of_errors[4]);

        fclose(file);
        fclose(mpddst);
        fclose(error_category);
        //fclose(runMClist);
        //fclose(recolist);
        return 0;
    }else
    {
        fprintf(stderr,"Wrong numer of arguments!\nExiting...\n");
        return 1;
    
    }
    
}