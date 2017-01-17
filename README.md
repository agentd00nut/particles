# particles
An exploration into particle graphics.

`Build.sh` Can be used to build this... I didn't do a make file becausue "meh".

`./particles`  Runs the simulation.  Change the parameters around lines 84-105 to do other things.

http://www.giphy.com/gifs/26gs62MGkZfTwFMBO

TODO
====

Make particles change color on speed.
Introduce drag... a pseudo "atmosphere" to slow particles down.


CONTRIBUTE PLEASE
=================

When particles get very close to the bodies that they are attracted to the math falls apart and all
of a sudden particles get a force of 100, 300, 35000 applied to them...

I've tried ignoring attraction if the distance is below some threshold, deleting particles that move too quickly,
trying a max force threshold, and a dynamic timestep that slows when particles are near gravity wells.

None of them worked in an acceptable way.

If you can look in `body.cpp` at the distance and attraction methods and make particles very close together not
go insane that'd be amazeballs.

< 25 distance things start getting speedy.
< 5 distance things become outrageos (applied forces of >100 );
< 1 distance things become broke dick ( 35,000 applied force );

 
