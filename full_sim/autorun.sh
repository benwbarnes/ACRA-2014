#!/bin/bash
#./bin/acra_experiment ./exp_3/ 30 0.001 0 | tee exp_3_lifetime.tsv
#./bin/acra_experiment ./exp_3/ 30 0.001 1 | tee exp_3_pt_mgmt.tsv
#./bin/acra_experiment ../video_capture/take2/ 50 0.001 0 | tee take2_lifetime.tsv
./bin/acra_experiment ../video_capture/take2/ 50 0.001 1 | tee take2_pt_mgmt.tsv
