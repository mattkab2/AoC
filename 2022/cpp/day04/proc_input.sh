#!/bin/sh
cat $1 | sed 's/[^[:digit:]]\+/ /g' > "${1::-3}dat"
