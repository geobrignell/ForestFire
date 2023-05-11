#!/bin/bash


for i in 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9
do  
    echo $i
    ./s_forest_fire.exe 500 $i 500 #1 size grid, #i p, #2 seed
    continue
done   

echo N $1 , S $2 done