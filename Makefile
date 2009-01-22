all: PLANET

clean:
	-rm *.o
	-rm PLANET
	-rm .data-stamp

.data-stamp: PLANET bodies
	./PLANET < bodies
	touch .data-stamp

plot: .data-stamp plot.gp
	./plot.gp

%: %.c
	gcc -Wall -lm -funroll-loops -g $< -o $@
