all: PLANET

clean:
	-rm *.o PLANET .data-stamp man earth sun moon plot.gif

.data-stamp: PLANET bodies
	./PLANET -f bodies
	touch .data-stamp

plot.gif: .data-stamp plot.gp loop.gp
	./plot.gp > plot.gif

display: plot.gif
	animate plot.gif&
%: %.c
	gcc -Werror -Wall -lm -funroll-loops -g $< -o $@
