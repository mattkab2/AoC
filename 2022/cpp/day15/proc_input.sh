#!/bin/sh
cat $1 | sed 's/Sensor at x=//g' | sed 's/, y=/ /g' | sed 's/ closest beacon is at x=//g' | sed 's/:/ /g'> "${1::-3}dat"