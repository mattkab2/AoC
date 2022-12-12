#!/bin/sh

cat $1 | sed 's/  Starting items: //g' | sed 's/,//g' | sed 's/  Operation: new = //g' | sed 's/  Test: divisible by //g' | sed 's/    If true: throw to monkey //g' | sed 's/    If false: throw to monkey //g' > "${1::-3}dat"