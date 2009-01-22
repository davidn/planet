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
#define MOVE_SUN 0
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
} body;

body * body_new(double mass, vector x, vector v)
{
	body * self = malloc(sizeof(body));
	self->mass = mass;
	self->x = calloc(2,sizeof(double));
	memcpy(self->x,x,2*sizeof(double));
	self->v = calloc(2,sizeof(double));
	memcpy(self->v,v,2*sizeof(double));
	return self;
}

void body_free(body * self)
{
	vector_free(self->x);
	vector_free(self->v);
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
#if MOVE_SUN
	vector r = vector_difference(self->x,neighbour->x);
#else
	vector r = self->x;
#endif
	double modr = vector_magnitude(r);
	double magnitude = G * neighbour->mass /
		square(modr);
	a[0] = -1 * r[0] * magnitude / modr;
	a[1] = -1 * r[1] * magnitude / modr;
	printf(" %f %f", a[0], a[1]);
	body_update_v(self,a);
	body_update_x(self);
#if MOVE_SUN
	vector_free(r);
#endif
	vector_free(a);
}

int main(int argc, char ** argv)
{
	body * sun = body_new(1.98892e30,vector_new(0,0),vector_new(0,0));
	body * earth = body_new(5.9742e24, vector_new(0,1.5e11),vector_new(3e4,0));
	int t;

	for (t = 0; t<tmax; t += dt )
	{
		printf("%f %f %f %f", earth->x[0],earth->x[1], earth->v[0], earth->v[1]);
		body_attract_to(earth, sun);
#if MOVE_SUN
		body_attract_to(sun, earth);
#endif
		printf("\n");
	}
	body_free(earth);
	body_free(sun);
	return 0;
}
