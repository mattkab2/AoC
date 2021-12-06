#!/bin/bash

nLines=`wc -l < 'input.txt'`
nLines=$nLines-1
echo $(( nLines / 6)) > "input.dat"

sed 's/,/ /g;s/  / /g' 'input.txt' >> 'input.dat'

