#!/usr/bin/gnuplot -persist

set term gif animate
unset border
set zeroaxis 7
set size square
size=1.5e11
set xrange [-size:size]
set yrange [-size:size]

length = 100000
da = 10000
a = 0
max = 3e5

load "loop.gp"
