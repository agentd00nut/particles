#include "body.h"
#include <math.h>
#include <stdlib.h>     /* fabs */


t_VECT Body::distance(Body other){
	float sx, sy, ox, oy, d, dx, dy;
	sx = position.x;
	sy = position.y;
	ox = other.position.x;
	oy = other.position.y;

	dx = (ox-sx);
	dy = (oy-sy);
	d = sqrt( pow(dx,2) + pow(dy,2)) ;

	if( d == 0 )
	{
		d=1;
		printf("WARNING: Distance between two bodies was 0, converting to .0001\n");
	}

	t_VECT dist(dx, dy, d);


	return dist;
}

t_VECT Body::attraction(Body other, int constant, int debug){
	
	t_VECT dist = this->distance(other);
	t_VECT force;



	force.d = constant * position.d * other.position.d / ( pow( dist.d, 2 ) );
	
	float theta = atan2(dist.y, dist.x);

	force.x = cos(theta) * force.d;
	force.y = sin(theta) * force.d;

	if(debug){
		printf("\ndX:%g, dY:%g\n", dist.x, dist.y);
		printf("fX:%g, fY:%g,  fD:%g\n", force.x, force.y, force.d);
		printf("theta: %g, cT: %g, sT: %g\n", theta, cos(theta)*force.d, sin(theta)*force.d);
	}

	//if(  fabs(force.x) > fabs(last_force.x*10)  || fabs(force.y) > fabs(last_force.y*10) ) 
	if(0)
	{
		force.x=0;
		force.y=0;
		force.d=dist.d;
		if(debug){
			printf("TOOCLOSE: fX:%g, lX:%g, fY:%g, lY:%g \n\n", force.x, last_force.x*2, force.y, last_force.y*2 );
			
		}

		return force;
	}

	force.d = dist.d;

	last_force.x = force.x;
	last_force.y = force.y;

	return force;

}

void Body::report(){
	printf("	POS: %g, %g\n", position.x, position.y);
	printf("	MASS: %g\n", position.d);
	printf("	VEL: %g, %g\n\n", velocity.x, velocity.y);
	printf("	COLOR: %i, %i, %i, %i \n", color.r, color.b, color.g, color.o);
	printf("	TRAIL: %i / %i\n", trail.n, trail.limit);
	return;
}

void Body::add_velocity(t_VECT new_velocity){
	velocity.x += new_velocity.x;
	velocity.y += new_velocity.y;
	return;
}

void Body::update(float timestep, float scale){
	position.x += velocity.x * timestep;
	position.y += velocity.y * timestep;
	trail.add(position.x*scale, position.y*scale);

}

void Body::draw(){
	glBegin(GL_LINE_STRIP);
    glColor4ub( color.r, color.b, color.g, color.o );

    for(int i=0; i < trail.path.size()-1 ; i++)
    {
        
        glVertex3f(  trail.path[i].x,  trail.path[i].y, 0 );
        
    }
    glEnd();
}