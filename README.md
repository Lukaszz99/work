
      Łukasz Sawicki           
   l.sawicki99@gmail.com       

To using these files you need to have access to NICA cluster and access to /eos/nica/mpd/data folder

With these files you can run jobs automatically with UrQMD and vHLLE simulation package (other in future). To run jobs is required to install UrQMD, vHLLE and MpdROOT, because for today (August 2019), they are not installed on cluster for everyone.

After clonning files from GitHub you need to change path to yours in these files in template folder
run_Urqmd.qsub
run_vHLLE.qsub
Then you have to make in template/programs and filetest/UrQMD and filetest/vHLLE (on august 2019 vHLLE filetest is still not ready)

How to run UrQMD simulation
1. Type ./run_jobs_UrQMD.sh energy number_of_simulations
avaliable are only intiger numbers (you can change it into float by editing source code)

How to run vHLLE simulation
1. You need to change energy and number of events in macro vHLLE.C in vHLLE/macro folder. Remember to set gen->SetOutputDirectory as "./" in this macro.
2. Type ./run_jobs_vHLLE.sh energy_typed_in_macro number_of_simulations

Data are stored in /eos/nica/mpd/data in diffrent folder for each energy and simulation package

TESTFILE
How to run
1. Type ./testfile path_to_data energy
Normally path_to_data is /eos/nica/mpd/data

This program is checking output files from simulation, macro runMC.C and reco.C. It tells wich simulation failed and why, printing list of correct mpddst.root files.
If macro runMC.C or reco.C failed there is txt file with comands to run again these macros. You need to copy text from it and paste in filetest/UrQMD/queuefiles.