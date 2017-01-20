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
		d=80;
		//printf("WARNING: Distance between two bodies was 0, converting to .0001\n");
	}

	t_VECT dist(dx, dy, d);


	return dist;
}

t_VECT Body::attraction(Body other, int constant, int debug){
	
	t_VECT dist = this->distance(other);
	t_VECT force;

	if(dist.d < 80)
	{
		dist.d = 80;
	}

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
	printf("	POS: %g, %g \n", position.x, position.y);
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

void Body::add_velocity(float x, float y){
	velocity.x += x;
	velocity.y += y;
	return;
}

void Body::update(vector<Body> &bodies, float &timestep, float &scale, float &magnetism, float &speed_full, float &speed_steps, int &opacity_step ){

	int b_size = bodies.size();
	float adjusted_mass = position.d * timestep;

	float total_fx =0;
	float total_fy =0;

	for(int i=0; i < b_size; i++){

	    t_VECT new_f = this->attraction(bodies[i] , magnetism, 0);
		total_fx += new_f.x;
		total_fy += new_f.y;
		
		//printf("New X: %g New Y: %g\n", new_f.x, new_f.y);
	}
	
	//printf("Total FX: %g Total FY: %g\n", total_fx, total_fy);
	
	total_fx = total_fx / adjusted_mass;
	total_fy = total_fy / adjusted_mass;    
	this->add_velocity( total_fx, total_fy );
	
	//printf("Velocity X: %g Velocity Y: %g\n", velocity.x, velocity.y);

	position.x += velocity.x * timestep;
	position.y += velocity.y * timestep;

	this->update_color(speed_full, speed_steps, opacity_step);

	trail.add(position.x * scale, position.y * scale, color.r, color.b, color.g, color.o);

	//this->report();
}

void Body::update_color(float &speed_full, float &speed_steps, int &opacity_step)
{
    float vel = fabs(velocity.x) + fabs(velocity.y);


    if( vel > speed_full ){ vel = speed_full; }
    
    float speed_color = vel * speed_steps;

    //cout << "Speed color was: " << speed_color << "\n";

    if( speed_color <= 255 ){
        color.r=0;
        color.g=0;
        color.b=speed_color;
    }
    else if( speed_color >= 255 && speed_color <= 510){
        color.r=0;
        color.g=speed_color-255;
        color.b=255-speed_color;
    }else{
        color.r=speed_color-510;
        color.g=510-speed_color;
        color.b=0;
    }

    color.o = opacity_step; // TODO: this should be moved so it's only set once when the body is made.

    //cout << "R: " << color.r << "G: " << color.g << "B: "<<color.b << "\n";
}

void Body::draw(){

	glBegin(GL_LINE_STRIP);
//    glColor4ub( color.r, color.g, color.b, color.o );
    int path_size = trail.path.size();

    for(int i=0; i < path_size ; i++)
    {   





    	glColor4ub(  trail.color_path[i].r, trail.color_path[i].g, trail.color_path[i].b, trail.color_path[i].o );
        glVertex3f(  trail.path[i].x,  trail.path[i].y, 0 );   
    }

    glEnd();
}