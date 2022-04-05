#!/bin/bash

echo `wc -l 'input.txt'` > 'input.dat'
sed "s/on/T/g;s/off/F/g;s/x=//g;s/y=//g;s/z=//g;s/,/ /g;s/\.\./ /g" "input.txt" >> "input.dat"
