///
///  @file PDynamics.cpp
///  @brief where the random parameters are calculated which affect the particles dynamics, all attributes
///  are influenced by the gameTimer so after a particle has lived for so long colour, size and velocity start to change

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
#include "PDynamics.h"

pDynamics::pDynamics()
{
  m_gameTimer=0.0f;
  m_sizeChange=0.0f;
  m_gravity.m_x=0.0;
  m_gravity.m_y=0.0f;
  m_gravity.m_z=0.0f;
  m_velocityChange.m_x=0.0f;
  m_velocityChange.m_y=0.0f;
  m_velocityChange.m_z=0.0f;
  m_velocityChange.m_w=0.0f;
  m_colourChange.m_r=0.0f;
  m_colourChange.m_g=0.0f;
  m_colourChange.m_b=0.0f;
  m_colourChange.m_a=0.0f;
}

//starts when mouse is clicked and increments the gametimer
void pDynamics::timer()
{
  m_gameTimer+=1.0;
}

//passing in _timer from main when mouse is clicked
void pDynamics::setTimer(const float _timer)
{
  m_gameTimer=_timer;
}

//function to return the game timer used in main function
float pDynamics::getTimer() const
{
  return m_gameTimer;
}

//used to reset gravity to 0 when mouse is clicked and new firework is created
void pDynamics::setGravity(const float _gravity)
{
  m_gravity.m_y=_gravity;
}

//used when the mouse is clicked so everytime a new firework is created it will have a random x gravity influence
float pDynamics::randomiseGravity()
{
  m_gravity.m_x=((float(rand()) / float(RAND_MAX)) * (0.1f - +0.8f)) + +0.8f;

  return m_gravity.m_x;
}

//randomises the rate at which particles are affected by curvature/gravity in pGeneration
float pDynamics::randomVarianceGravity()
{
  m_randomVarianceGrav=((float(rand()) / float(RAND_MAX)) * (0.8f - +1.35f)) + +1.35f;

  return m_randomVarianceGrav;
}

//gravity wont change per perticle will be acting on the object as a group
//calculates the ejection and affects gravity on all the particles for the y direction
Vec4 pDynamics::gravity()
{
  //needs to start with negative y for upwards particle movement then change to send particles downwards
  //works on a curved model base, start fast, slow down when going upwards then stop, then accelerates
  //on the way down as gravity takes over
  //initially the ejection influence

  if(m_gameTimer>=0 && m_gameTimer<15)
  {
    m_gravity.m_y-=0.15;
  }
  else if(m_gameTimer>=16 && m_gameTimer<22)
  {
    m_gravity.m_y-=0.1;
  }
  else if(m_gameTimer>=23 && m_gameTimer<27)
  {
    m_gravity.m_y-=0.070;
  }
  else if(m_gameTimer>=28 && m_gameTimer<32)
  {
    m_gravity.m_y-=0.065;
  }
  else if(m_gameTimer>=33 && m_gameTimer<36)
  {
    m_gravity.m_y-=0.04;
  }
  else if(m_gameTimer>=37 && m_gameTimer<43)
  {
    m_gravity.m_y-=0.03;
  }
  else if(m_gameTimer>=44 && m_gameTimer<49)
  {
    m_gravity.m_y-=0.01;
  }
  //where ejection force has been overcome by forces of gravity and particles start to fall
  else if(m_gameTimer>=50 && m_gameTimer<51)
  {
    m_gravity.m_y+=0.0;
  }
  //switch gravity counter so particles start to come down, increasing acceleration with time
  else if(m_gameTimer>=52 && m_gameTimer<56)
  {
    m_gravity.m_y+=0.005;
  }
  else if(m_gameTimer>=57 && m_gameTimer<60)
  {
    m_gravity.m_y+=0.15;
  }
  else if(m_gameTimer>=61 && m_gameTimer<65)
  {
    m_gravity.m_y+=0.20;
  }
  else if(m_gameTimer>=66 && m_gameTimer<73)
  {
    m_gravity.m_y+=0.3;
  }
  else if(m_gameTimer>=74)
  {
    m_gravity.m_y+=0.5;
  }

  return m_gravity;
}

//function to return a random velocity change value for each particle which will be used in the draw function
//after a certain game time
Vec4 pDynamics::velocityChange()
{
  m_velocityChange.m_x=((float(rand()) / float(RAND_MAX)) * (0.3f - -0.3f)) + -0.3f;
  m_velocityChange.m_y=((float(rand()) / float(RAND_MAX)) * (0.1f - +0.3f)) + +0.3f;

  return m_velocityChange;
}

//returns a random colour change for each particle to be used in draw function also
Vec4 pDynamics::colourChange()
{
  m_colourChange.m_r=((float(rand()) / float(RAND_MAX)) * (0.5f - +1.5f)) + +1.5f;
  m_colourChange.m_g=((float(rand()) / float(RAND_MAX)) * (0.5f - +1.5f)) + +1.5f;
  m_colourChange.m_b=((float(rand()) / float(RAND_MAX)) * (0.5f - +1.5f)) + +1.5f;

  return m_colourChange;
}

//returns a random size change for each particle also to be used in draw function
float pDynamics::sizeChange()
{
  m_sizeChange=((float(rand()) / float(RAND_MAX)) * (0.7f - +1.3f)) + +1.3f;

  return m_sizeChange;
}


