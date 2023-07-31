#!/bin/bash
#SBATCH --mail-type=ALL
#SBATCH --account=carney-tserre-condo
#SBATCH --mail-user=mohit_vaishnav@brown.edu
#SBATCH -n 5
#SBATCH --mem=50G
#SBATCH -N 1
# Specify an output file
#SBATCH -o ../../slurm/%j.out
#SBATCH -e ../../slurm/%j.err

# Specify a job name:
#SBATCH --time=100:00:00


#  svrt is the ``Synthetic Visual Reasoning Test'', an image generator
#  for evaluating classification performance of machine learning
#  systems, humans and primates.
#
#  Copyright (c) 2009 Idiap Research Institute, http://www.idiap.ch/
#  Written by Francois Fleuret <francois.fleuret@idiap.ch>
#
#  This file is part of svrt.
#
#  svrt is free software: you can redistribute it and/or modify it
#  under the terms of the GNU General Public License version 3 as
#  published by the Free Software Foundation.
#
#  svrt is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
#  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
#  License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with svrt.  If not, see <http://www.gnu.org/licenses/>.

source ~/vreason/bin/activate
# module load python/3.7.4
module load cuda/10.1.105
echo $CUDA_VISIBLE_DEVICES
module load libpng12/1.2.57

nb_samples_to_save=50000
# nb_samples_for_training=110000

problem_list=$*

[[ ${problem_list} ]] || problem_list=$(echo {1..23})

set -e

make -j -k vision_test

for problem_number in ${problem_list}; do

    # result_dir=../../svrt_dataset/results_problem_${problem_number}/train/
    # result_dir0=../../svrt_dataset/results_problem_${problem_number}/train/cat_0
    # result_dir1=../../svrt_dataset/results_problem_${problem_number}/train/cat_1
    result_dir=../../svrt_dataset/results_problem_${problem_number}/train/
    result_dir0=../../svrt_dataset/results_problem_${problem_number}/train/cat_0
    result_dir1=../../svrt_dataset/results_problem_${problem_number}/train/cat_1

    mkdir -p ${result_dir}
    mkdir -p ${result_dir0}
    mkdir -p ${result_dir1}

    ./vision_test \
        --problem_number=${problem_number} \
        --nb_train_samples=${nb_samples_to_save} \
        --result_path=${result_dir} \
        --random_seed=0 \
        write-samples

    # ./vision_test \
    #     --problem_number=${problem_number} \
    #     --nb_train_samples=${nb_samples_for_training} \
    #     --result_path=${result_dir} \
    #     --progress_bar=no \
    #     randomize-train adaboost compute-train-error compute-test-error | tee ${result_dir}/log
done
