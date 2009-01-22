all: PLANET

clean:
	-rm *.o
	-rm PLANET
	-rm .data-stamp

.data-stamp: PLANET bodies
	./PLANET < bodies
	touch .data-stamp

plot.gif: .data-stamp plot.gp loop.gp
	./plot.gp > plot.gif

%: %.c
	gcc -Wall -lm -funroll-loops -g $< -o $@
