#!/bin/sh
cat $1 | sed 's/,/ /g' > "${1::-3}dat"