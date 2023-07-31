#!/bin/bash
pathd=$(pwd)
for i in results_problem_*; do
	echo $i
	pathd+="/"	
	pathd+=$i
	echo PATHD=${pathd}
	pathd=$(pwd)
done
