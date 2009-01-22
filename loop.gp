#!/usr/bin/gnuplot

a = a + da

if (a<length) plot "man" every ::0::a title "man" w lines, "earth" every ::0::a title "earth" w lines; else plot "man" every ::(a-length)::a title "man" w lines, "earth" every ::(a-length)::a title "earth" w lines;

if (a<max) reread
