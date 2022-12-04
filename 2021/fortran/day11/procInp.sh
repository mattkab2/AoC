#!/bin/bash

x=`wc -l "input.txt"`
line=`head -n 1 input.txt`
y=${#line}
echo "$y $x" > "input.dat"
sed 's/./& /g' "input.txt" >> "input.dat"
