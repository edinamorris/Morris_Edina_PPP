Particle System - firework generator

This program relies on SDL2, OpenGL gl.h, and glu.h 

This particle system generates fireworks when the left mouse button is clicked on screen. Open up the project, the main.cpp file is where you want to be as there are certain values that can be changed by the user to affect the way the firework looks. Such as:

- The mean amount of particles and the maximum variance from that number. To change this go to main.cpp and scroll down to line 134, change the value in this line:    	numberParticles.setMeanParticle(250); 
to change the mean number of particles created initially. 

- the maximum variance is line 136, this is used when calculating the random number of particles it will be the maximum either side (positive and negative) of the mean number.
  numberParticles.setVarianceParticle(20);
so for example with 250 particles the variance as 20 means the number of particles for each firework will be as high as 270 or as low as 230.

- Also the mean colours of the firework in RGB can be altered. the code is on line 138. the values of red, green and blue are between 0.0 and 1.0. with 1.0 being full red and 0.0 being no red influence. 
  Particle.setMeanColour(0.6f,0.2f,0.4f);
So these values mean there will be a higher influence of red than the others, and about twice as much influence of blue to green (is also affected by random numbers after this).

- Finally the user can affect the mean position of the particles relative to the mouse click origin position. The values entered on line 140 will be the particles rough position and will vary based on that. This means not all particles will generate in exactly the same position there will be some randomness to it.
  Particle.setMeanPosition(0.02, 0.03, 0.07);
the format is x, y, z.

Once you have set these variables to the values you like then simply run the program and click on the screen with the left mouse button!
