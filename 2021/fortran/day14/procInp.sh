#!/bin/bash

grep -o '\->' "input.txt" | wc -l > "input.dat"
sed 's/-> //g;/^$/d' "input.txt" >> "input.dat"
