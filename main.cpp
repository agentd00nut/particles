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

	// GL INIT

	srand(time(NULL));
    int width = 800, height = 800, depth = 200;

    sf::Window window(sf::VideoMode(width, height), "OpenGL", sf::Style::Default, sf::ContextSettings(32));

    sf::Clock Clock;

    glClearDepth(1.f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_TRUE);


    //// Setup a perspective projection & Camera position


    glMatrixMode(GL_PROJECTION);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_LINE_SMOOTH); 
    glLoadIdentity();
    perspectiveGL(90.f, 1.f, 1.f, 2000.f);//fov, aspect, zNear, zFar
	

    //// COLORS


    t_COLOR red(255,0,0,255);
    t_COLOR green(0,255,0,255);
    t_COLOR blue(0,0,255,255);
    t_COLOR white(255,255,255,255);
    t_COLOR yellow(189,183,107,255);
    t_COLOR salmon(250,128,114, 255);
    t_COLOR thisshit(32,178,170, 255);




	// GRAVITY INIT

    float magnetism = 1000;  // Constant that controls magnitude of attraction.
    int trail_length = 2;   // Controls how many steps particles remember, must be > 1
    float timestep = 1;     // "Accuracy" of simulation... .1 is 10x slower but 10x more accurate (probably) 
    //float accurate_timestep = 1; // Left over from when particles close to gravity wells would slow down timestep loop.


    //// Create Gravity wells... these are the "Interesting" things.
    //  Techinically these don't need a color or trail_length but i don't feel like making the Body accept that.
    bodies.push_back( Body( 800, 0, 10,    0, 0 , 0,   blue, trail_length ));
    bodies.push_back( Body( -800, 0, 10,    0, 0 , 0,   blue, trail_length ));
    bodies.push_back( Body( 00, 800, 10,    0, 0 , 0,   blue, trail_length ));
    bodies.push_back( Body( 00, -800, 10,    0, 0 , 0,   blue, trail_length ));


    // Create lots of particles... 20000 to be exact.
    // Between -500 and 500 X and Y... pretty arbitrary choice of starting position.
    for(int i =0; i < 20000; i++)
    {
        particles.push_back( Body( rand()%1000 - 500, rand()%1000 - 500, .1,    0, 0, 0,   green, trail_length ));
    }



    // MAIN LOOP INIT

    int running = 1;    
    int i, j;

    Body current_body;
    t_VECT new_f;
    t_VECT new_v;

    float total_fx;
    float total_fy;

    sf::Event event;

    int zoom=-1000;


     // Controls stuff ( it mostly doesn't work )
    float x,y;
    x=0;
    y=0;
    int xP = 0, yP = 0;
    int xM= 0, yM =0;
    float mouseX =0, mouseY=0;
    float old_mouseX =0, old_mouseY=0;
    bool pressedLeft = false, pressedRight = false;
    int lxM=0, lyM=0;



    // MAIN LOOP

    int debug = 0;
    while(running){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();


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
                zoom-=10;
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


        // UPDATE POSITIONS

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


            // This is a poor attempt at finding a straight forward way to delete super fast particles.
            // It doesn't really work.
            // "100" seems to be better but meh idk...
            if(  current_body.velocity.x < -magnetism || current_body.velocity.y < -magnetism || current_body.velocity.x > magnetism || current_body.velocity.y > magnetism || ( current_body.position.x < -10000 || current_body.position.x > 10000 ) || ( current_body.position.y < -10000 || current_body.position.y > 10000 )){
                
                particles.erase(particles.begin()+i);
                i-=1;
            } 


            // Actual Drawing to the screen.

			glBegin(GL_LINE_STRIP);
		    glColor4ub( current_body.color.r, current_body.color.b, current_body.color.g, 255);
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
