#!/bin/bash

awk -F'-' '{print $1} {print $2}' "input.txt" | sort | uniq | wc -l > "input.dat"
sed 's/-/ /g' "input.txt" >> "input.dat"
