#!/usr/bin/gnuplot -persist

plot "data" u 1:2 title "sun" w lines, "data" u 3:4 title "earth" w lines
