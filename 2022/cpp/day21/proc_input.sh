#!/bin/sh
cat $1 | sed 's/://' > "${1::-3}dat"