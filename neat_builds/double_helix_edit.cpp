#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <unistd.h>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

using namespace std;

#include "body.cpp"
//#include "trail.h"

void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan( fovY / 360 * pi ) * zNear;
    fW = fH * aspect;

    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}





float AU = (800);

//float SCALE = 2 / AU;
//float SCALE = .01;
float SCALE = 800 /AU;
vector<Body> bodies;
vector<Body> particles;

int main(int argc, char** argv)
{


	//Body a(0,0,10, 0,0,0, t_COLOR(255,0,0,255), 123);
	

	// GL INIT

	srand(time(NULL));
    int width = 800, height = 800, depth = 200;

    sf::Window window(sf::VideoMode(width, height), "OpenGL", sf::Style::Default, sf::ContextSettings(32));

    //window.setFramerateLimit(fps);
    sf::Clock Clock;

    // load resources, initialize the OpenGL states, ...
    glClearDepth(1.f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.f);
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_TRUE);

    //// Setup a perspective projection & Camera position
    glMatrixMode(GL_PROJECTION);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_LINE_SMOOTH); 
    glLoadIdentity();
    perspectiveGL(90.f, 1.f, 1.f, 200000.f);//fov, aspect, zNear, zFar
	

    t_COLOR red(255,0,0,255);
    t_COLOR green(0,255,0,255);
    t_COLOR blue(0,0,255,255);
    t_COLOR white(255,255,255,255);
    t_COLOR yellow(189,183,107,255);
    t_COLOR salmon(250,128,114, 255);
    t_COLOR thisshit(32,178,170, 255);
	// GRAVITY INIT

    float magnetism = 1000;
    int trail_length = 2;
    float timestep = 1;
    float accurate_timestep = 1;



    bodies.push_back( Body( 0, -400, 6,    0, 0 , 0,   blue, trail_length ));
    bodies.push_back( Body( 0, 0,   6,     0, 0 , 0,   blue, trail_length ));
    bodies.push_back( Body( 0, 800, 25,     0, 0 , 0,   blue, trail_length ));
    //bodies.push_back( Body( 00, 200, 10,    0, 0 , 0,   blue, trail_length ));
    //bodies.push_back( Body( 00, -200, 10,    0, 0 , 0,   blue, trail_length ));
   // bodies.push_back( Body( 100, 0, 1000,    0, 0 , 0,   blue, trail_length ));

    for(int i =0; i < 10000; i++)
    {
        particles.push_back( Body(  0 -400 , rand()%6000-3000 -4000, .1,  0,3, 0,   green, trail_length ));
        particles.push_back( Body( 0 +400 , rand()%6000-3000 -4000, .1,  0, 3, 0,   green, trail_length ));
    }

    printf("woo\n");

    //Body a(-1, 1, 5.9742 * pow(10,24),    0,0,0, red,  trail_length); // earth
    //Body b(, 1, 1.98892 * pow(10,24),    0,0,0, blue, trail_length); // sun
/*
    float magnetism = 10;
    int trail_length = 100000;
    float timestep = 1;
    float accurate_timestep = .6;
    bodies.push_back( Body( -501, 0, .001,    0, 10 , 0,   red, trail_length ));
    bodies.push_back( Body( -301, 0, .001,    0, 10 , 0,   green, trail_length ));
    
    bodies.push_back( Body( -400, 0, 1000,    0, 2.5, 0,   white, trail_length ));



    bodies.push_back( Body( 501, 0, .001,    0, -10 , 0,   salmon, trail_length ));
    bodies.push_back( Body( 301, 0, .001,    0, -10 , 0,   thisshit, trail_length ));
    
    bodies.push_back( Body( 400, 0, 1000,    0, -2.5, 0,   yellow, trail_length ));
*/
    //bodies.push_back( Body( 0, 0, 1000,    .01, -.01, 0,   blue, trail_length ));
    //bodies.push_back( Body(0,     0,  1000000,   0,   0,0, red,  trail_length) );
    //bodies.push_back( Body(10,     0,  100,   0,   800,0, green,  trail_length) );

    //bodies.push_back( Body(-1*AU, 0, 5.9742*pow(10,24),  0, 29.783 * 1000, 0, red, trail_length) ); // earth
   // bodies.push_back( Body(1*AU, 0, 5.9742*pow(10,24),  0, 12.783 * 1000, 0, red, trail_length) ); // earth
    //bodies.push_back( Body(-1*AU+(0.3633*pow(10,6)), 10000, 0.07346*pow(10,24),  0, (29.783 * 1000)+1.022*1000, 0, white, trail_length) ); // moon
    //bodies.push_back( Body(0, 0, 1.98892*pow(10,30),  0, 0, 0, green, trail_length) ); // sun
    //bodies.push_back( Body(0.723 * AU, 0, 4.8685*pow(10,24),  0, -35.02 * 1000, 0, green, trail_length) ); // venus
    //bodies.push_back( Body(-1*AU, 0, 1.98892*pow(10,30),  0, 15.783 * 1000, 0, green, trail_length) ); // sun
    //bodies.push_back( Body(1*AU, 0, 1.98892*pow(10,30),  0, -15.783 * 1000, 0, red, trail_length) ); // sun



    // loop init

int running = 1;    
    int i, j;

    Body current_body;
    t_VECT new_f;
    t_VECT new_v;

    float total_fx;
    float total_fy;

    sf::Event event;




    float x,y;
    x=0;
    y=0;
    int xP = 0, yP = 0;
    int xM= 0, yM =0;
    float mouseX =0, mouseY=0;
    float old_mouseX =0, old_mouseY=0;
    bool pressedLeft = false, pressedRight = false;

    int lxM=0, lyM=0;

    int zoom=-1000;

    // DRAWING RESET
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();


            // GRAVITY LOOP 

    int debug = 0;
    float vel=0;
        int r, b, g;

        float speed_full=15;
        float speed_steps=765/speed_full; //765 = 255 * 3
        float speed_color;

    while(running){


    	// DRAWING RESET
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();


        //window.display();

        //printf("%g, %g\n", x,y);

        // END DEBUG POINT 

	    // CONTROLS 


        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
            else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
            {
                window.close();
            }
            else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Q))
            {
                zoom+=200;
            }
            else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::W))
            {
                zoom-=100;
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                //mouseX = sf::Mouse::getPosition(window).x;
                //mouseY = sf::Mouse::getPosition(window).y;

                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    printf("L_Pressed: %d %d\n",event.mouseButton.x, event.mouseButton.y );
                   
                    pressedLeft = true;
                    old_mouseX = event.mouseButton.x;
                    old_mouseY = event.mouseButton.y;
                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    printf("R_Pressed: %d %d\n",event.mouseButton.x, event.mouseButton.y );
                    
                    pressedRight = true;
                    old_mouseX = mouseX;
                    old_mouseY = mouseY;
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    printf("L_Released: %d %d\n",event.mouseButton.x, event.mouseButton.y );
                    pressedLeft = false;

                    old_mouseX = mouseX;
                    old_mouseY = mouseY;
                    lxM=xM;
                    lyM=yM;
                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    printf("R_Released: %d %d\n",event.mouseButton.x, event.mouseButton.y );
                
                    pressedRight = false;
                    

                    old_mouseX = mouseX;
                    old_mouseY = mouseY;
                }
            }

            if(pressedLeft && !pressedRight){
                if (event.type == sf::Event::MouseMoved)
                {
                    xM = event.mouseMove.x - old_mouseX;
                    yM = event.mouseMove.y - old_mouseY;
                    printf("%i %i  %i %i   %f %f\n", xM, yM, event.mouseMove.x, event.mouseMove.y, old_mouseX, old_mouseY);

                }
            }
            if(pressedRight && !pressedLeft){
                if (event.type == sf::Event::MouseMoved)
                {
                    
                    xP = event.mouseMove.x + (event.mouseMove.x - old_mouseX) ;
                    yP = event.mouseMove.y + (event.mouseMove.y - old_mouseY );

                }

                old_mouseX = mouseX;
                old_mouseY = mouseY;
            }
        }

        glTranslatef(xM-lxM, yM-lyM, zoom);

    	// EXERT FORCE ON EVERY BODY AND UPDATE POSITIONS.

        
    	for(i=0; i<particles.size(); i++){

    		current_body = particles[i];
    		total_fx = 0;
    		total_fy = 0;
    		new_v.x=0;
    		new_v.y=0;
            
            
            for(j=0; j<bodies.size(); j++){

                new_f = current_body.attraction(bodies[j] , magnetism,debug);
    			total_fx += new_f.x;
    			total_fy += new_f.y;
            }
                //printf("new_f.d: %g\n", new_f.d);
   

                /*if(new_f.d < 50 && ts_smaller==0)
                {
                    old_ts=timestep;
                    timestep=accurate_timestep;
                    i=0;
                    j=0;
                    ts_smaller=1;
                    nosmall=0;
                    goto lmfao_omg;

                }*/

            
            //printf("timestep: %f\n", timestep);
            new_v.x += total_fx / current_body.position.d * timestep ;
            new_v.y += total_fy / current_body.position.d * timestep ;    
        
    		current_body.add_velocity(new_v);
    		current_body.update(timestep, SCALE);
    		//current_body.report();
    		//current_body.draw();


            particles[i] = current_body;

            if(  current_body.velocity.x < -magnetism || current_body.velocity.y < -magnetism || current_body.velocity.x > magnetism || current_body.velocity.y > magnetism || ( current_body.position.x < -10000 || current_body.position.x > 10000 ) || ( current_body.position.y < -10000 || current_body.position.y > 10000 )){
                
                particles.erase(particles.begin()+i);
                i-=1;
                //particles.push_back( Body( rand()%2000 - 1000, rand()%2000 - 1000, 1, 0, 0, 0,   red, trail_length ));
                //particles.resize()

                //printf("erased particle, new size: %lu \n", particles.size());
            } 

			glBegin(GL_LINE_STRIP);

            vel = fabs(current_body.velocity.x) + fabs(current_body.velocity.y);


            if( vel > speed_full ){ vel = speed_full; }
            speed_color = vel * speed_steps;
            //printf("%g %g \n", vel, speed_color);
            if( speed_color <= 255 ){
                r=0;
                g=0;
                b=speed_color;
            }
            else if( speed_color > 255 && speed_color <= 510){
                r=0;
                g=speed_color-255;
                b=255-speed_color;
            }else{
                r=speed_color-510;
                g=510-speed_color;
                b=0;
            }


		    //glColor4ub( current_body.color.r, current_body.color.b, current_body.color.g, 255);
            glColor4ub( r, g, b, 255);
		    for(int f=0; f < current_body.trail.path.size()-1 ; f++)
		    {
		        glVertex3f(  current_body.trail.path[f].x,  current_body.trail.path[f].y, 0 );
            }
		        
		    
		    glEnd();

        


    	}

    	// DRAW THAT SHIT YO

    	window.display();
	    

    }
}
