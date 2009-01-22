/*  PLANET - A Orbit Simulation
 * (c) 2009 David Newgas
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define dt 100
#define tmax 3e7
#define G 6.673e-11
#define square(a) ((a)*(a))
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
	char is_static;
} body;

body * body_new(char * name, double mass, vector x, vector v, char is_static)
{
	body * self = malloc(sizeof(body));
	self->name = name;
	self->mass = mass;
	self->x = calloc(2,sizeof(double));
	memcpy(self->x,x,2*sizeof(double));
	self->v = calloc(2,sizeof(double));
	memcpy(self->v,v,2*sizeof(double));
	self->is_static = is_static;
	return self;
}

void body_free(body * self)
{
	vector_free(self->x);
	vector_free(self->v);
	/*free(self->name); /* Don't do for non dynamic strings! */
	free(self);
}

void body_update_x(body * self)
{
	self->x[0] += self->v[0] * dt;
	self->x[1] += self->v[1] * dt;
}

void body_update_v(body * self, vector acceleration)
{
	self->v[0] += acceleration[0] * dt;
	self->v[1] += acceleration[1] * dt;
}

void body_attract_to(body * self, body * neighbour)
{
	vector a = vector_new(0,0);
	vector r = vector_difference(self->x,neighbour->x);
	double modr = vector_magnitude(r);
	double magnitude = G * neighbour->mass /
		square(modr);
	a[0] = -1 * r[0] * magnitude / modr;
	a[1] = -1 * r[1] * magnitude / modr;
	/*printf(" %f %f", a[0], a[1]);*/
	body_update_v(self,a);
	vector_free(r);
	vector_free(a);
}

int main(int argc, char ** argv)
{
	body * bodies[] = {
		body_new("sun",1.98892e30,vector_new(0,0),vector_new(0,0), 1),
		body_new("earth",5.9742e24, vector_new(0,1.5e11),vector_new(3e4,0),0)
	};
	int no_bodies = 2;
	long int t;
	int i,j;
	for (t = 0; t<tmax; t += dt )
	{
		for (i=0;i<no_bodies; i++)
		{
			printf("%f %f ", bodies[i]->x[0],bodies[i]->x[1]);
			if (! bodies[i]->is_static)
			{
				for(j=0;j<no_bodies;j++)
					if(i != j)
						body_attract_to(bodies[i], bodies[j]);
				body_update_x(bodies[i]);
			}
		}
		printf("\n");
	}
	body_free(bodies[0]);
	body_free(bodies[1]);
	return 0;
}
