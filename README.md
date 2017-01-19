# particles
An exploration into particle graphics.

`Build.sh` Can be used to build this... I didn't do a make file becausue "meh".

`./particles`  Runs the simulation.  Change the parameters around lines 84-105 to do other things.

http://www.giphy.com/gifs/26gs62MGkZfTwFMBO

DONE
====

Change color based on particles speed. A bit sloppy but it works.
Fixed particles "blasting off" when very close to sources of gravity.
	Protip: 1 / ( < 1 ^ 2 ) ===  >1 !

Added opacity to particle trails, acts as a "heat map".
	Change `opacity_step` to 255, and `trail_length` to 2 if you like particles.

If you really like JUST particles with no trails at all change `trail_length` to 1 and
`GL_LINE_STRIP` to `GL_POINTS` for a speed boost.

TODO
====

Threading, I'd like to move all of the position update code into the body class So we can
thread the update logic for each particle.  This would lead to inaccuracies (particle didn't update)
and i'm curious how many threads you can make before shit blows up but it'd clean up the code anyways.

Look into different ways to draw particle paths... I'm 95% sure I could restructure how the path
is stored so that I could do something like `draw_function( THE WHOLE FLIPPIN PATH )` instead of
iterating over each particles path and drawing each point one by one oh my god it's slow.

CONTRIBUTE
==========

It'd be so neat if someone could change the panning code to actually pan correctly.
Really if someone could point me to a good beginners tutorial on translating the camera, that'd be neat.

