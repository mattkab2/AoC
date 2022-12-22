#!/bin/sh
cat $1 | sed 's/Valve //g' | sed 's/has flow rate=//g' | sed 's/; tunnel leads to valve//g' | sed 's/; tunnels lead to valves//g' | sed 's/,//g' > "${1::-3}dat"