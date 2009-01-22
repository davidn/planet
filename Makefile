all: PLANET

clean:
	-rm *.o
	-rm PLANET

data: PLANET bodies
	./PLANET < bodies > data

plot: data plot.gp
	./plot.gp

%: %.c
	gcc -Wall -lm -funroll-loops -g $< -o $@
