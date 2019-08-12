#!/bin/bash

source /eos/nica/mpd/users/sawicki/mpdroot/build/config.sh #change path to your own root
root -b -q runMC.C
#cp /eos/nica/mpd/users/zielinski/work/TestEvUrqmd.template/reco.C .
root -b -q reco.C
