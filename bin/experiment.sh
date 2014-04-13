#!/bin/bash
for num_points in 10 50 100 150 200 250
do
	./LKTrack $num_points off | tee $num_points.txt
done
