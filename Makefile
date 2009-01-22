all: PLANET

clean:
	-rm *.o
	-rm PLANET

data: PLANET
	./PLANET > data

plot: data plot.gp
	./plot.gp

%: %.c
	gcc -Wall -lm -funroll-loops -g $< -o $@
