#!/usr/bin/gnuplot -persist

set term gif animate
unset border
set zeroaxis 7
set size square
size=6.7e6
set xrange [-size:size]
set yrange [-size:size]

length = 100
da = 100
a = 0
max = 3e5

load "loop.gp"
