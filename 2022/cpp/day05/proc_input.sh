#!/bin/sh
cat $1 | sed 's/from//g' | sed 's/to//g' | sed 's/move//g' > "${1::-3}dat"