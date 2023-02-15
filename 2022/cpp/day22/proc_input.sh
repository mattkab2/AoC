#!/bin/sh
cat $1 | sed 's/L/ L /g; s/R/ R /g' > "${1::-3}dat"