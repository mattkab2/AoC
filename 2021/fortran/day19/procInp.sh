#!/bin/bash

n=`grep -o 'scanner' "input.txt" | wc -l`
echo $n > "input.dat"
arr=`awk '!/^---/{count++}/^---/{print count; count = -1}' 'input.txt'`
echo $arr >> "input.dat"
sed '/^$/d;/^--/d' 'input.txt' >> 'input.dat'
