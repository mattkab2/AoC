#!/bin/bash

grep '^[0-9]' 'input.txt' | wc -l > 'input.dat'
grep 'fold along' 'input.txt' | wc -l >> 'input.dat'
sed 's/,/ /g;s/fold along //g;s/=/ /g;s/x/T/g;s/y/F/g' 'input.txt' >> 'input.dat'
