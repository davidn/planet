#!/usr/bin/gnuplot -persist

plot "data" u 1:2 title "sun" w points, "data" u 3:4 title "earth" w lines, "data" u 5:6 title "moon" w lines
