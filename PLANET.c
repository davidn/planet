/*  PLANET - A Orbit Simulation
 * (c) 2009 David Newgas <dn271@cam.ac.uk>
 */

/*
 *  Guide to Source code.
 *  
 * This is essentially Euler-style numerical approximation.  There are two 
 * classes:
 *
 * Vector - this is simply a more covienient array of doubles. It has a few
 * utility functions that are pretty obvious.
 *
 * Body - This is the important object - it stores information about each body
 * in the system.  More importantly, it had body_attract_to, which updates it's
 * velocity according to the acceleration to the other body.  It is called for 
 * every body on every other body.  This means out algorithm is O(n^2)
 * Body also has body_update_x which is called every iteration to move the body.
 *
 * We are relying on the linearity of newton's laws by updating velocity immidiately
 * for every other body.  Otherwise we must keep track of the overall force
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

#define G 6.673e-11
#define MAX_BODIES 5
#define square(a) ((a)*(a))
#define DEFAULT_TMAX 3e7
/* All measurements in SI units */

typedef double * vector;

vector vector_new(double x, double y)
{
	vector self = malloc(2* sizeof(double));
	self[0] = x;
	self[1] = y;
	return self;
}

void vector_free(vector self)
{
	free(self);
}
vector vector_difference(vector self, vector neighbour)
{
	vector difference = vector_new(0,0);
	int i;
	for (i = 0 ; i <=1; i++) /* thank god for -funroll-loops */
	{
		difference[i] = self[i]-neighbour[i];
	}
	return difference;
}
double vector_magnitude( vector self )
{
	int i;
	double magnitude=0;
	for (i = 0 ; i <=1; i++) /* thank god for -funroll-loops */
	{
		magnitude += square(self[i]);
	}
	return sqrt(magnitude);

}

typedef struct
{
	double mass;
	vector x;
	vector v;
	char * name;
	int is_static;
} body;

body * body_new(char * name, double mass, vector x, vector v, char is_static)
{
	body * self = malloc(sizeof(body));
	self->name = name;
	self->mass = mass;
	self->x = x;
	self->v = v;
	self->is_static = is_static;   /* This allows us to save a bit of calculation
									  on bodies that are static.*/
	return self;
}

void body_free(body * self)
{
	vector_free(self->x);
	vector_free(self->v);
	free(self->name); /* Don't do for non dynamic strings! */
	free(self);
}

void body_update_x(body * self, int dt)
{
	self->x[0] += self->v[0] * dt;
	self->x[1] += self->v[1] * dt;
}

void body_update_v(body * self, int dt, vector acceleration)
{
	self->v[0] += acceleration[0] * dt;
	self->v[1] += acceleration[1] * dt;
}

void body_attract_to(body * self, int dt, body * neighbour)
/* Here we work out the acceleration (=force/mass) on the body, and then update
   v
   Note that we calculate modr and magnitude as they are used mroe than once.  with luck
	 the compiler will optimise it a bit.*/
{
	vector a = vector_new(0,0);
	vector r = vector_difference(self->x,neighbour->x);
	double modr = vector_magnitude(r);
	double magnitude = G * neighbour->mass /
		square(modr);
	a[0] = -1 * r[0] * magnitude / modr;
	a[1] = -1 * r[1] * magnitude / modr;
	/*printf(" %f %f", a[0], a[1]);*/
	body_update_v(self, dt ,a);
	vector_free(r);
	vector_free(a);
}

void euler_run (long long tmax, int timestep, body * bodies[], FILE * outputs[], int no_bodies)
{
	long long t;
	int i,j;
	/* loop for out iterations */
	for (t = 0; t<tmax; t += timestep )
	{
		for (i=0;i<no_bodies; i++)
		{
			/* print out details at start to get initial conditions in output */
			fprintf(outputs[i],"%f\t%f\n", bodies[i]->x[0],bodies[i]->x[1]);
			if (! bodies[i]->is_static)
			{
				for(j=0;j<no_bodies;j++)
					if(i != j)
						body_attract_to(bodies[i], timestep, bodies[j]); /* Do this on everything */
				body_update_x(bodies[i], timestep); /* but do this once */
			}
		}
	}
}

void leapfrog_run (long long tmax, int timestep, body * bodies[], FILE * outputs[], int no_bodies)
{
	long long t = 0;
	int i,j;
	timestep /= 2;
	/* loop for out iterations */
	while ( t<tmax )
	{
		for (i=0;i<no_bodies; i++)
		{
			/* print out details at start to get initial conditions in output */
			fprintf(outputs[i],"%f\t%f\n", bodies[i]->x[0],bodies[i]->x[1]);
			if (! bodies[i]->is_static)
			{
				body_update_x(bodies[i],timestep);
			}
		}
		t += timestep;
		for (i=0;i<no_bodies; i++)
		{
			if (! bodies[i]->is_static)
			{
				for(j=0;j<no_bodies;j++)
					if(i != j)
						body_attract_to(bodies[i], timestep * 2, bodies[j]);
				body_update_x(bodies[i],timestep);
			}
		}
		t += timestep;
	}
}

int main(int argc, char ** argv)
{
	FILE * input = stdin;
	body * bodies[MAX_BODIES];
	FILE * outputs[MAX_BODIES];
	int no_bodies;
	long long tmax = DEFAULT_TMAX;
	int opt;
	int dt = 100;
	int method = 1; /* 0=euler, 1=leapfrog */
	
	while ((opt=getopt (argc,argv,"f:i:helt:s:"))!=-1)
	{
		switch (opt)
		{
			case 'f':
				input = fopen(optarg,"r");
				if (input == NULL)
			{
				printf("Could not open %s for reading\n",optarg);
				exit(1);
			}
				break;
			case 's':
				dt = atoi(optarg);
				break;
			case 't':
				tmax = atoll(optarg);
			case 'e':
				method = 0;
				break;
			case 'l':
				method = 1;
				break;
			default:
			fprintf (stderr, "Unrecognised option: %d\n",opt);
			case '?':
			case 'h':
			fprintf(stderr, "Usage: %s [-f infile] [-s timestep] [-t max_time] [-e | -l]\n",argv[0]);
			exit(1);
		}
	}
	
	/* Get bodies from input: */
	for (no_bodies=0; !feof(input) && no_bodies<MAX_BODIES ;no_bodies++)
	{
		/* We call body_new before getting the details to save storing it all in
		 temp variables. NB: %as is using the GNU extension "a" modifier.  Saves
		 some work and neatness.  it requires the string to be freed.*/
		bodies[no_bodies] = body_new(NULL,0,vector_new(0,0),vector_new(0,0),0);
		fscanf(input, "%as %lf %lf %lf %lf %lf %d\n",
				&bodies[no_bodies]->name,
				&bodies[no_bodies]->mass,
				&bodies[no_bodies]->x[0],
				&bodies[no_bodies]->x[1],
				&bodies[no_bodies]->v[0],
				&bodies[no_bodies]->v[1],
				&bodies[no_bodies]->is_static);
		outputs[no_bodies] = fopen(bodies[no_bodies]->name,"w");
		if (outputs[no_bodies]==NULL)
		{
			/* should be done with errno etc */
			printf("Could not open %s for writing\n", bodies[no_bodies]->name);
			exit(1);
		}

	}
	if ( method == 0 )
	{
		euler_run( tmax, dt, bodies, outputs, no_bodies);
	}
	else
	{
		leapfrog_run( tmax, dt, bodies, outputs, no_bodies);
	}
	for (; no_bodies > 0 ; no_bodies--)
		body_free(bodies[no_bodies-1]); /* unecessary, but good practice */
	return 0;
}
