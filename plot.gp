#!/usr/bin/gnuplot -persist

plot "data" u 1:2 w lines, "data" u 1:2:($3*10000):($4*10000) every 1000 w vector, "data" u 1:2:($5*10000):($6*10000) every 1000 w vector
