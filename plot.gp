#!/usr/bin/gnuplot -persist

set term wxt persist 
unset border
set zeroaxis 7
set size square
plot "sun" w lines, "earth" w lines, "moon" w lines
