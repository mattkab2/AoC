#!/bin/bash

wc -l "input.txt" > "input.dat"
sed 's/./& /g' "input.txt" >> "input.dat"
