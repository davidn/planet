#!/usr/bin/gnuplot -persist

set term gif animate
unset border
set zeroaxis 7
set size square
set xrange [-1.6e11:1.6e11]
set yrange [-1.6e11:1.6e11]

length = 100000
da = 10000
a = 0
max = 3e5

load "loop.gp"
