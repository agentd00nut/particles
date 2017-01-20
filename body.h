#include <iostream>
using namespace std;
typedef struct t_VECT {

    float x;
    float y;
    float d;


    t_VECT() : x(0), y(0), d(0){}
    t_VECT(float x, float y, float d) : x(x), y(y), d(d) {}
    t_VECT(float x, float y): x(x), y(y), d(0){}
    
} t_VECT;


typedef struct t_COLOR {

    int r;
    int b;
    int g;
    int o;


    t_COLOR() : r(255), b(255), g(255), o(255){}
    t_COLOR(int r, int b, int g, int o) : r(r), b(b), g(g), o(o){}
    
} t_COLOR;


typedef struct t_TRAIL {

	vector<t_VECT> path;
    vector<t_COLOR> color_path;
	int limit;
	int n;


	t_TRAIL(): path(), limit(100), n(0){}
	t_TRAIL(int limit) : path(), limit(limit), n(0){}


	void add(float x, float y, float r, float b, float g, float o){

		this->path.push_back( t_VECT( x, y ) );
        this->color_path.push_back( t_COLOR( r, b, g, o ) );

		if(n > limit){
			this->path.erase( path.begin() );
            this->color_path.erase( color_path.begin() );
		}else
		{
			n++;
		}
	}

} t_TRAIL;


class Body {
public:
	float mass;
	t_VECT velocity;
	t_VECT position;
	t_COLOR color;
	t_TRAIL trail;

	t_VECT last_force;

	t_VECT attraction(Body , int, int);
	t_VECT distance(Body);


	Body():velocity(0,0,0), position(0,0,0), color(255,255,255,255), trail(100){} 
	Body( float px, float py, float pd, float vx, float vy, float vd, t_COLOR color, int trail) : velocity(vx, vy, vd), position(px,py,pd), color(color), trail(trail), last_force(1000000,100000,100000) {};

	void report();
	//void prep_update(t_VECT position; t_VECT velocity) : new_position(position), new_velocity(velocity){}
	void add_velocity(t_VECT new_velocity);
	void add_velocity(float x, float y);

    void update_color(float &speed_full, float &speed_steps,  int &opacity_step);

    void update(vector<Body> &bodies, float &timestep, float &scale, float &magnetism, float &speed_full, float &speed_steps,  int &opacity_step);

	void draw();
};