#!/bin/bash

y=`wc -l input.txt`
line=`head -n 1 input.txt`
x=${#line}
echo "$x $y" > "input.dat"

sed 's/\./0 /g;s/\#/1 /g' 'input.txt' >> 'input.dat'
