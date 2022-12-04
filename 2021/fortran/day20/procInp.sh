#!/bin/bash

wc -l "input.txt" > "input.dat"
sed 's/\./F /g;s/#/T /g' "input.txt" >> "input.dat"
