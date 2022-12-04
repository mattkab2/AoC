#!/bin/bash

x=`wc -l input.txt`
line=`head -n 1 input.txt`
y=${#line}

echo "$y $x" > "input.dat"
sed 's/\./ 0 /g;s/>/ 1 /g;s/v/ -1 /g' 'input.txt' >> 'input.dat'
