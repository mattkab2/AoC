#!/bin/sh
cp $1 "${1::-3}dat"
echo -en "\nnoop" >> "${1::-3}dat"