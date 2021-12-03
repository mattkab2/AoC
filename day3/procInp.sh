#!/bin/bash

x=`wc -l input.txt`
line=`head -n 1 input.txt`
y=${#line}

echo "$y $x" > "input.dat"
sed 's/0/0 /g;s/1/1 /g' 'input.txt' >> 'input.dat'
