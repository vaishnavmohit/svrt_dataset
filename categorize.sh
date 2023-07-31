#!/bin/bash
mkdir -p train/cat_1 #create directory if doesnt exists
mkdir -p train/cat_0
mkdir -p test/cat_1
mkdir -p test/cat_0
mkdir -p val/cat_1
mkdir -p val/cat_0
mv /train/sample_1_* /train/cat_1/
mv /train/sample_0_* /train/cat_0/

:''
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
