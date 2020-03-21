Łukasz Sawicki           
l.sawicki99@gmail.com       
Jakub Zielinski
qba.zielinski@gmail.com

#Prerequisites

To be able to use these files you need to have access to NICA cluster and access to /eos/nica/mpd/data folder.

With these files you can run jobs automatically with UrQMD and vHLLE simulation package (other in future). In order to run jobs it is required to install UrQMD, vHLLE and MpdROOT, because for today (August 2019), they are not installed on cluster for everyone.
(Update: new files are not configured yet for vHLLE.)

After cloning files from GitHub you need to change path to yours in these files in template folder: run_Urqmd.qsub  and run_vHLLE.qsub.
(Update: updating run_vHLLE.qsub is not important right now)

#How to run the UrQMD simulation

To run jobs with the UrQMD model you need two files from this repository: runCommands.sh and run_UrQMD.qsub (must be in the same directory).
Run the runCommands.sh script and follow instructions in the terminal.

#How to run vHLLE simulation

Not inmplemented yet!


#Directory structure
Data are stored in /eos/nica/mpd/data in different folders. Files are stored in directories given by the rule:
/eos/nica/mpd/data/NUCLEI-NUCLEI/MODEL/ENERGY/IMP/JOB_ID/

(example: job 1074283 for Bi-Bi collision @ ecm = 9 GeV with max_imp=3 fm 500 events: /eos/nica/mpd/data/Bi-Bi/UrQMD/9GeV/0-3fm/1074283/ )

mpddst.root and mpddst.MiniDST.root are sotred in TestEvUrqmd in each JOB_ID folder. They are also linked to the rootfiles folder that is on the same level as JOB_ID directories. MiniDST files are linked to the rootfiles/MiniDST/. Link is named with all parameters used during job's configuration (example: BiBi.ecm9gev.imp0-3fm.500.dst.ID1074283.root)


Used simulation package
UrQMD 3.4
vHLLE https://github.com/pbatyuk/vHLLE_package