#!/bin/bash


for j in 0, 1, 2, 3, 4
do
    for i in  0.1, 0.2, 0.3, 0.4, 0.5, 0.51, 0.52, 0.53, 0.54, 0.55, 0.56, 0.57, 0.58, 0.59, 0.595 0.599, 0.6, 0.605 0.61, 0.62. 0.63, 0.64, 0.65, 0.7, 0.8, 0.9, 1.0
    do  
        echo $i
        ./wind.exe 500 $i $j #1 size grid, #i p, #2 wind
        continue
    done   

    echo w = $j finished
done
echo Completed!