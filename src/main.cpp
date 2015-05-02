///
///  @file main.cpp
///  @brief where main game functions, drawing and random functions are called.
///  The main focus of my project was to create a firework system that was realistic by having a lot of random
///  variables that affect each individual particle. for example I did this by randomising the end of the partciles
///  lifetime, the time at which a particle starts to die is random from others, and the rate at which they die
///  is also based on a random number. This way it looks as realistic as possible and not mechanical.
///  I set up a way for all of these variables to be stored in a
///  vector of structures which are then passed into the draw function. Initially in my design I had all of the
///  functions returning values within the draw function which wasn't very efficient and didn't work with large
///  particle numbers. This was because it was taking so long to run all the functions for the particle. So I
///  changed it to the draw function receiving all the values when it gets  to that particle number. This way all
///  the random values are stored for all the particles before the drawing function is even called. This takes the load
///  off the program as all values are good to go when the particles need to be drawn!
/// \Revision History:
/// \todo make more varaibles changeable by the user in here in one place (already some from line 132 to 141)

#include <SDL.h>
#ifdef LINUX
	#include <GL/gl.h>
#endif
#ifdef DARWIN
	#include <OpenGL/gl.h>
#endif

#include <iostream>
#include <string>
#include <vector>
#include "GLFunctions.h"
#include "Mat4.h"
#include "PAttributes.h"
#include "PDynamics.h"
#include "PGeneration.h"
#include "PUpdate.h"

/// @brief function to quit SDL with error message
/// @param[in] _msg the error message to send
void SDLErrorExit(const std::string &_msg);


/// @brief initialize SDL OpenGL context
SDL_GLContext createOpenGLContext( SDL_Window *window);

/**
 * @brief main
 * @return exit code
 */

/// This is the structure mentioned in the brief above, it is part of the system that stores the random
/// numbers for the particles.
//firework structure used to store n number of values for each variable to be used to draw the fireworks,
//where n is the random number of particles assigned for the current firework
typedef struct
{
  Vec4 colour;
  Vec4 position;
  Vec4 mousePosition;
  Vec4 velocity;
  Vec4 randvelocity;
  int increment;
  float lifeTime;
  float transparencyCounter;
  float lowerTransparency;
  float sizeChange;
  float randomGravity;
  float randomVarianceGravity;
  Vec4 speedChange;
  Vec4 colourChange;
}randomDraw;

/// the vector mentioned in the brief above, used in the main focus of my project
//creating a vector which is dynamic so that a variable number of values can be pushed back onto the list
std::vector<randomDraw> randomdraw;

const int killTime=110;

randomDraw fireworkAttributes(pGeneration numberParticles, pAttributes Particle, pUpdate Velocity, pDynamics dynamics);

int main()
{
  // Initialize SDL's Video subsystem
  if (SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    // Or die on error
    SDLErrorExit("Unable to initialize SDL");
  }

  // now get the size of the display and create a window we need to init the video
  SDL_Rect rect;
  SDL_GetDisplayBounds(0,&rect);
  // now create our window
  SDL_Window *window=SDL_CreateWindow("PARTICLE SYSTEM",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      rect.w/2,
                                      rect.h/2,
                                      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
                                     );
  // check to see if that worked or exit
  if (!window)
  {
    SDLErrorExit("Unable to create window");
  }

  // Create our opengl context and attach it to our window

   SDL_GLContext glContext=createOpenGLContext(window);
   if(!glContext)
   {
     SDLErrorExit("Problem creating OpenGL context");
   }
   // make this our current GL context (we can have more than one window but in this case not)
   SDL_GL_MakeCurrent(window, glContext);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable( GL_BLEND );
  /* This makes our buffer swap syncronized with the monitor's vertical refresh */
  SDL_GL_SetSwapInterval(1);
  // now clear the screen and swap whilst NGL inits (which may take time)

  //particle variables
  // flag to indicate if we need to exit
  bool quit=false;
  //setting various counters to initially be 0
  int mouseClick=0;
  //increment is used for the movement of particles in the 1st wave
  float increment=0;
  //incrementNextWave is used for the movement of particles in the 2nd wave
  float incrementNextWave=0;
  //nextWaveCounter is used to know when to start the 2nd wave of particles
  int nextWaveCounter=0;
  //deleteCounter is used to know when to kill all of the remaining particles in the firework system
  int deleteCounter=0;
  //The timer that has to be reached by the nextWaveCounter to start the next wave
  int nextWaveTimer=25;
  //value used to increment the gravity in the x direction for the particles
  float gravityXIncrementer=1.0;


  //create instances
  pAttributes Particle;
  std::vector<pGeneration> pgeneration;
  pGeneration numberParticles;
  pUpdate Velocity;
  pDynamics dynamics;

  //*****-------------------EDIT PARAMETERS HERE--------------------*****//
  //change mean number of particles
  numberParticles.setMeanParticle(250);
  //change maximum variance from the mean number of particles (actual variance also relies on a random number)
  numberParticles.setVarianceParticle(20);
  //change red, green, blue mean variables for particles
  Particle.setMeanColour(0.6f,0.2f,0.4f);
  //change mean start position in x,y,z direction of the particle from the origin of mouse click
  Particle.setMeanPosition(0.02, 0.03, 0.07);
  //*****---------------------END EDITING HERE----------------------*****//

  glClear(GL_COLOR_BUFFER_BIT);
  GLFunctions::perspective(50, float(1024/720),0.1,100);
  GLFunctions::lookAt(Vec4(0,0,-1),Vec4(0,0,0),Vec4(0,-1,0));

  SDL_GL_SwapWindow(window);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glColor3f(1,1,0);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);

  // sdl event processing data structure
  SDL_Event event;

  // now draw particles
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // now we create an instance of our ngl class, this will init NGL and setup basic
  // opengl stuff ext. When this falls out of scope the dtor will be called and cleanup
  // our gl stuff
  while(!quit)
  {
    while ( SDL_PollEvent(&event) )
    {
      //draw particles if left mouse button is clicked
      if (event.type == SDL_MOUSEBUTTONDOWN)
      {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
          //screen position assigned to mouseX and mouseY
          //all counters/incrementers also get reset when mouse is clicked
          float mouseX, mouseY;
          mouseX=event.button.x;
          mouseY=event.button.y;
          mouseClick=1;
          dynamics.setTimer(0.0);
          dynamics.setGravity(0.0);
          dynamics.randomiseGravity();
          nextWaveCounter=0;
          increment=0;
          incrementNextWave=0;
          deleteCounter=0;
          gravityXIncrementer=1.0;
          for(int i=0; i<numberParticles.numberOfParticles(); i++)
          {
            randomdraw[i].transparencyCounter=0;
          }
          //screen position mouse location sent into function to calculate object world coordinates
          Particle.setMousePosition(mouseX, mouseY);
          //calls function to get random values for all the particles and assign them to the randomdraw
          //structure to be used to draw the particles
          fireworkAttributes(numberParticles, Particle, Velocity, dynamics);
        }
      }
      switch (event.type)
      {
        // this is the window x being clicked.
        case SDL_QUIT : quit = true; break;
        // if the window is re-sized pass it to the ngl class to change gl viewport
        // note this is slow as the context is re-create by SDL each time

        // now we look for a keydown event
        case SDL_KEYDOWN:
        {
          switch( event.key.keysym.sym )
          {
            // if it's the escape key quit
            case SDLK_ESCAPE :  quit = true; break;
            case SDLK_w : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
            case SDLK_s : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;

            default : break;
          } // end of key process
        } // end of keydown

        default : break;
      } // end of event switch
    } // end of poll events

    //gravity
    Vec4 gravity = dynamics.gravity();
    //game timer to be passed into the draw function for size, colour and velocity change
    float gameTimer = dynamics.getTimer();

    /// As mentioned in the brief above, this is where all the random functions are called and passed into draw
    /// this is also where the particle lifetime calculations are done
    //where the draw function is called and all the parameters passed in
    for(int i=0; i<numberParticles.numberOfParticles(); i++)
    {
      //push back each particle onto the vector
      randomdraw.push_back(randomDraw());
      //only if the mouse has been clicked will the ejection function be called, as this function contains
      //counters and they shouldn't start increasing until the firework is created
      randomdraw[i].randvelocity=Velocity.randomVelocity();
      //start assigning the current particles random values to the parameters passed into the draw function
      float r=randomdraw[i].colour.m_r;
      float g=randomdraw[i].colour.m_g;
      float b=randomdraw[i].colour.m_b;
      //if a certain time has passed then start lowering the transparency of the particles (killing them)
      //each particle has a slighlty varied lifetime based around a mean number
      //the amount at which they decrease is also slightly random for each particle based around a mean incremental value
      if(deleteCounter>=(randomdraw[i].lifeTime))
      {
        randomdraw[i].lowerTransparency=1;
        randomdraw[i].transparencyCounter+=Velocity.randomLifeTimeIncrementer();
      }
      //the transparency value that gets passed to draw is a random starting transparency number plus the
      //varying counter, and each particle starts dying at different times
      if(randomdraw[i].lowerTransparency==1)
      {
        randomdraw[i].colour.m_a=Velocity.lowerTransparency()+(randomdraw[i].transparencyCounter);
      }
      //continue assigning the current particles random values to the parameters passed into the draw function
      float a=randomdraw[i].colour.m_a;
      float x=randomdraw[i].position.m_x;
      float y=randomdraw[i].position.m_y;
      float z=randomdraw[i].position.m_z;
      Vec4 mousePosition=randomdraw[i].mousePosition;
      float randVelocityX=randomdraw[i].randvelocity.m_x+increment;
      float randVelocityY=randomdraw[i].randvelocity.m_y+increment;
      float randGravityX=randomdraw[i].randomGravity;
      if(gameTimer<60)
      {
        randGravityX=1;
      }
      float gravityX=gravityXIncrementer*randGravityX;
      if(gravityX<1.0)
      {
        gravityX=1.0;
      }
      float randomVarianceGravity=randomdraw[i].randomVarianceGravity;
      //xoffset calculated using effects by gravity and random number
      float xOffset=randomdraw[i].velocity.m_x*randVelocityX*gravityX;
      float yOffset=randomdraw[i].velocity.m_y*randVelocityY;
      float sizechange=randomdraw[i].sizeChange;
      Vec4 speedchange=randomdraw[i].speedChange;
      Vec4 colourchange=randomdraw[i].colourChange;

      //draw particles
      pgeneration[i].draw(r, g, b, a, x, y, z, mousePosition.m_x, mousePosition.m_y, xOffset, yOffset,
                          gravity, sizechange, speedchange, colourchange, gameTimer, randomVarianceGravity);
    }

    //where the draw function is called for a 2nd time for the 2nd wave of particles
    if(nextWaveCounter>=nextWaveTimer)
    {
      //only start movement incrementer when 2nd wave has been released
      incrementNextWave+=0.5;
      for(int i=numberParticles.numberOfParticles()+1; i<numberParticles.numberOfParticles()*2; i++)
      {
        //push back 2nd wave onto randomdraw vector as well
        randomdraw.push_back(randomDraw());

        randomdraw[i].randvelocity=Velocity.randomVelocity();
        float r=randomdraw[i].colour.m_r;
        float g=randomdraw[i].colour.m_g;
        float b=randomdraw[i].colour.m_b;
        if(deleteCounter>=(randomdraw[i].lifeTime))
        {
          randomdraw[i].lowerTransparency=1;
          randomdraw[i].transparencyCounter+=Velocity.randomLifeTimeIncrementer();
        }
        if(randomdraw[i].lowerTransparency==1)
        {
          //first wave should die quicker than 2nd wave, so 2nd wave counter has been reduced
          randomdraw[i].colour.m_a=Velocity.lowerTransparency()+(randomdraw[i].transparencyCounter*0.6);
        }
        float a=randomdraw[i].colour.m_a;
        float x=randomdraw[i].position.m_x;
        float y=randomdraw[i].position.m_y;
        float z=randomdraw[i].position.m_z;
        Vec4 mousePosition=randomdraw[i].mousePosition;
        float randVelocityX=randomdraw[i].randvelocity.m_x+incrementNextWave;
        float randVelocityY=randomdraw[i].randvelocity.m_y+incrementNextWave;
        float randGravityX=randomdraw[i].randomGravity;
        //only after the fireworks starts to drop down will gravityX be used in affecting the xOffset passed into draw
        if(gameTimer<60)
        {
          randGravityX=1;
        }
        float gravityX=gravityXIncrementer*randGravityX;
        //gravityX must have a value of minimum 1 so it doesnt affect xOffset negatively and switch particle direction
        if(gravityX<1.0)
        {
          gravityX=1.0;
        }
        float randomVarianceGravity=randomdraw[i].randomVarianceGravity;
        float xOffset=randomdraw[i].velocity.m_x*randVelocityX*gravityX;
        float yOffset=randomdraw[i].velocity.m_y*randVelocityY;
        float sizechange=randomdraw[i].sizeChange;
        Vec4 speedchange=randomdraw[i].speedChange;
        Vec4 colourchange=randomdraw[i].colourChange;

        //draw particles of 2nd wave too
        pgeneration[i].draw(r, g, b, a, x, y, z, mousePosition.m_x, mousePosition.m_y, xOffset, yOffset,
                            gravity, sizechange, speedchange, colourchange, gameTimer, randomVarianceGravity);
      }
    }

    //only if the mouse has been clicked start the incrementers
    if(mouseClick==1)
    {
      //used to determine whether it is time to delete the remaining particles in the firework
      deleteCounter+=1;
      //used in calculating x and y offset for first wave
      increment+=0.5;
      //used to determine whether next wave should be released
      nextWaveCounter+=1;
      //start game timer in dynamics class to be used to determine when to start changing size, colour etc...
      dynamics.timer();
      if(gameTimer>=60)
      {
        gravityXIncrementer+=0.03;
      }
    }

    //for each firework the exact time when all the remaining particles are deleted is randomized
    //around a mean killTime
    int deleteTimer=killTime+((float(rand()) / float(RAND_MAX)) * (10.0f - -10.0f)) + -10.0f;

    //if the counter gets higher than the randomized timer then the vector randomdraw is cleared
    if(deleteCounter>=deleteTimer)
    {
      randomdraw.clear();
      //also all counters are reset for the next firework
      deleteCounter=0;
      for(int i=0; i<numberParticles.numberOfParticles(); i++)
      {
        randomdraw[i].lowerTransparency=0;
        randomdraw[i].transparencyCounter=0;
      }
    }

    // swap the buffers
    SDL_GL_SwapWindow(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  }

  // now tidy up and exit SDL
  SDL_Quit();
}

SDL_GLContext createOpenGLContext(SDL_Window *window)
{
  // Request an opengl 3.2 context first we setup our attributes, if you need any
  // more just add them here before the call to create the context
  // SDL doesn't have the ability to choose which profile at this time of writing,
  // but it should default to the core profile
  // for some reason we need this for mac but linux crashes on the latest nvidia drivers
  // under centos
  #ifdef DARWIN
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  #endif
  // set multi sampling else we get really bad graphics that alias
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,2);
  // Turn on double buffering with a 24bit Z buffer.
  // You may need to change this to 16 or 32 for your system
  // on mac up to 32 will work but under linux centos build only 16
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  // enable double buffering (should be on by default)
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  //
  return SDL_GL_CreateContext(window);

}

/**
 * @brief SDLErrorExit
 * @param _msg
 */

/// This is the function mentioned in the brief above, it returns the vector of structures random values to main
//function which assigns n number of values to the randomDraw structure, where n is the random number of
//particles assigned for the current firework, returns the assigned structure to be used in main
randomDraw fireworkAttributes(pGeneration numberParticles, pAttributes Particle, pUpdate Velocity, pDynamics dynamics)
{
  for(int i=0; i<numberParticles.numberOfParticles()*2; i++)
  {
    randomdraw[i].colour=Particle.randomColour();
    randomdraw[i].position=Particle.randomPosition();
    randomdraw[i].mousePosition=Particle.getMousePosition();
    randomdraw[i].velocity=Velocity.initialVelocity();
    randomdraw[i].lifeTime=Velocity.particleLifeTime(killTime);
    randomdraw[i].sizeChange=dynamics.sizeChange();
    randomdraw[i].speedChange=dynamics.velocityChange();
    randomdraw[i].randomGravity=dynamics.randomiseGravity();
    randomdraw[i].colourChange=dynamics.colourChange();
    randomdraw[i].randomVarianceGravity=dynamics.randomVarianceGravity();
  }

  for(int i=0; i<numberParticles.numberOfParticles(); i++)
  {
    return randomdraw[i];
  }
  return randomdraw[0];
}

void SDLErrorExit(const std::string &_msg)
{
  std::cerr<<_msg<<"\n";
  std::cerr<<SDL_GetError()<<"\n";
  SDL_Quit();
  exit(EXIT_FAILURE);
}
