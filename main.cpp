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


double r_small()
{
    return (double)rand() / (double)RAND_MAX ;
}




float SCALE = 1;
vector<Body> bodies;
vector<Body> particles;

int main(int argc, char** argv)
{	

	// GL INIT

	srand(time(NULL));
    int width = 1200, height = 800, depth = 200;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::Window window(sf::VideoMode(width, height), "OpenGL", sf::Style::Default, settings);

    sf::Clock Clock;

    // load resources, initialize the OpenGL states, ...
    glClearDepth(1.f);
    glClearColor(0.05f, 0.05, 0.05, 0.f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_TRUE);

    glMatrixMode(GL_PROJECTION);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_LINE_SMOOTH); 
    glLoadIdentity();
    perspectiveGL(90.f, 1.f, 1.f, 2000.f);//fov, aspect, zNear, zFar
	

    t_COLOR red(255,0,0,255);
    t_COLOR green(0,255,0,255);
    t_COLOR blue(0,0,255,255);
    t_COLOR white(255,255,255,255);
    t_COLOR yellow(189,183,107,255);
    t_COLOR salmon(250,128,114, 255);
    t_COLOR thisshit(32,178,170, 255);
	


    // GRAVITY INIT

    float magnetism = 1000;
    int trail_length = 2; // 1000
    float timestep  = .6;
    
    int zoom=-700;
    
    float speed_full = 24; // for coloring.
    int opacity_step = 16;


    bodies.push_back( Body( -200, 0,   20,  0, 0 , 0,   blue, trail_length ));
    bodies.push_back( Body( 200, 0,   20,  0, 0 , 0,   blue, trail_length ));
    bodies.push_back( Body( 0, 0,   -10,  0, 0 , 0,   blue, trail_length ));


   
    for(int i =0; i < 4000; i++)
    {
        
        particles.push_back( Body(  (rand()%3001-1500)+r_small(), (rand()%401-200)+r_small()+400, 1,    2, -2, 0,   green, trail_length ));
        particles.push_back( Body(  (rand()%3001-1500)+r_small(), (rand()%401-200)+r_small()+400, 1,    -2, -2, 0,   green, trail_length ));
        particles.push_back( Body(  (rand()%3001-1500)+r_small(), (rand()%401-200)+r_small()-400, 1,    2, 2, 0,   green, trail_length ));
        particles.push_back( Body(  (rand()%3001-1500)+r_small(), (rand()%401-200)+r_small()-400, 1,    -2, 2, 0,   green, trail_length ));

    }

    



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



    // GRAVITY LOOP 

    int debug = 0;
    float vel=0;
    int r, b, g;

    float speed_steps=765/speed_full; //765 = 255 * 3
    float speed_color;

    while(running){


    	// DRAWING RESET
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();



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
                
            new_v.x += total_fx / current_body.position.d * timestep ;
            new_v.y += total_fy / current_body.position.d * timestep ;    
        
    		current_body.add_velocity(new_v);
    		current_body.update(timestep, SCALE);



            particles[i] = current_body;

          

            vel = fabs(current_body.velocity.x) + fabs(current_body.velocity.y);


            if( vel > speed_full ){ vel = speed_full; }
            speed_color = vel * speed_steps;

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

            glBegin(GL_LINE_STRIP);
            glColor4ub( r, g, b, opacity_step);
		    for(int f=0; f < current_body.trail.path.size() ; f++)
		    {
		        glVertex3f(  current_body.trail.path[f].x,  current_body.trail.path[f].y, 0 );
            }
          
		    
		    glEnd();

    	}

        /*glBegin(GL_POINTS);
        for(j=0; j<bodies.size(); j++){
             glColor4ub( 255, 0, 0, 255);
             glVertex3f( bodies[j].position.x,   bodies[j].position.y, 0 );
        }
        glEnd();
        */

    	window.display();
	    

    }
}
