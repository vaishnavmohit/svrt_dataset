#!/bin/bash
for i in results_problem_*; do
	echo $i
	pathd="/media/storage/vaishnav/svrt_dataset/"	
	pathd+=$i
	echo PATHD=${pathd}
	cd ${pathd}
	mkdir -p train/cat_1 #create directory if doesnt exists
	mkdir -p train/cat_0
	mkdir -p test/cat_1
	mkdir -p test/cat_0
	mkdir -p val/cat_1
	mkdir -p val/cat_0
	mv sample_1_* train/cat_1/
	mv sample_0_* train/cat_0/

:'
	N=40000
	cd train/cat_1/
	ls | shuf -n $N -e sample_1_* | xargs -i mv {} ../../test/cat_1/
	ls | shuf -n $N -e sample_1_* | xargs -i mv {} ../../val/cat_1/	

	cd ../cat_0/
	ls | shuf -n $N -e sample_0_* | xargs -i mv {} ../../test/cat_0/
	ls | shuf -n $N -e sample_0_* | xargs -i mv {} ../../val/cat_0/
'
done
