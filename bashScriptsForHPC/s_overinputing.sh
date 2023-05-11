#!/bin/bash

echo "Size" $1

for i in 1000
do
    ./s_inputing.sh $1 $i
    continue
done

echo Completed