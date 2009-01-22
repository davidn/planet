#!/usr/bin/gnuplot

a = a + da

if (a<length) plot "sun" every ::0::a title "sun" w lines, "earth" every ::0::a title "earth" w lines, "moon" every ::0::a title "moon" w lines; else plot "sun" every ::(a-length)::a title "sun" w lines, "earth" every ::(a-length)::a title "earth" w lines, "moon" every ::(a-length)::a title "moon" w lines

if (a<max) reread
