all: PLANET

clean:
	-rm *.o PLANET .data-stamp man earth sun moon plot.gif *~ planet.tar.gz

.data-stamp: PLANET bodies
	./PLANET -f bodies
	touch .data-stamp

plot.gif: .data-stamp plot.gp loop.gp
	./plot.gp > plot.gif

archive: planet.tar.gz

planet.tar.gz: PLANET.c bodies plot.gp loop.gp Makefile README TODO awk
	tar -zcf planet.tar.gz PLANET.c bodies plot.gp loop.gp Makefile awk README TODO

display: plot.gif
	gnome-open plot.gif&
%: %.c
	gcc -Werror -Wall -lm -funroll-loops -g $< -o $@
