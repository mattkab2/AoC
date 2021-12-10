#!/bin/bash

x=`cat "input.txt" | tr -cd , | wc -c`
echo $((x+1)) > 'input.dat'
sed 's/,/ /g' 'input.txt' >> 'input.dat'
