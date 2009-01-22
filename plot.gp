#!/usr/bin/gnuplot -persist

set term wxt persist 
unset border
set zeroaxis 7
set size square
plot "data" u 1:2 title "sun" w lines, "data" u 3:4 title "earth" w lines, "data" u 5:6 title "moon" w lines
