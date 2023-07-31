#!/bin/bash
pathd=$(pwd)
for i in results_problem_*; do
	
	echo $i
	pathd='/home/mvaishn1/Downloads/svrt-HEAD-d34ac2b/'
	pathd+=$i
	echo PATHD=${pathd}
	cd ${pathd}
	echo $(pwd)
	mkdir -p train/cat_1 #create directory if doesnt exists
	mkdir -p train/cat_0
	mkdir -p test/cat_1
	mkdir -p test/cat_0
	mkdir -p val/cat_1
	mkdir -p val/cat_0
	
	for file in sample_1_*; do 
		mv $file train/cat_1/
	done 

	for file in sample_0_*; do
		mv $file train/cat_0/		
	done

:'
	N=40000
	cd train/cat_1/
	ls |sort -R |tail -$N |while read file; do mv $file ../../test/cat_1/
	    # Something involving $file, or you can leave
	    # off the while to just get the filenames
	
	done

	cd ../cat_0/
	ls |sort -R |tail -$N |while read file; do mv $file ../../test/cat_0/
	    # Something involving $file, or you can leave
	    # off the while to just get the filenames
	
	done

	cd ../train/cat_1/
	ls |sort -R |tail -$N |while read file; do mv $file ../../val/cat_1/
	    # Something involving $file, or you can leave
	    # off the while to just get the filenames
	
	done

	cd ../cat_0/
	ls |sort -R |tail -$N |while read file; do mv $file ../../val/cat_0/
	    # Something involving $file, or you can leave
	    # off the while to just get the filenames
	
	done
	pathd=$(pwd)
'
done
