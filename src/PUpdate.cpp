///
///  @file PUpdate.cpp
///  @brief where the random attributes are generated for the time immediately after the particles are created
///  and deal with the constant updating of the particles movemenet, lifetimes and deaths

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
#include "PUpdate.h"

pUpdate::pUpdate()
{
  m_randomTransparency=0.0;
  m_initialLifeTime=0.0;
  m_meanIncrementer=0.0;
  m_randomIncrementer=0.0;
  m_initialIncrementer=0.0;
  m_velocity.m_x=0.0;
  m_velocity.m_y=0.0;
  m_velocity.m_z=0.0;
  m_randVelocity.m_x=0.0;
  m_randVelocity.m_y=0.0;
  m_randVelocity.m_z=0.0;
}

//returns a random lifeTime based on the killtime set it main, will return a randomised value based around
//2/5 of the fireworks overall life, this is so each particle will start to die at diffirent rates
float pUpdate::particleLifeTime(float killTime)
{
  m_meanLifeTime=(killTime)*0.4;
  m_randomLifeTime=((float(rand()) / float(RAND_MAX)) * (20.0f - -20.0f)) + -20.0f;
  m_initialLifeTime=(m_meanLifeTime+(m_randomLifeTime));

  return m_initialLifeTime;
}

//returns a random incrmenter so each particle's rate of decay will also vary slightly
float pUpdate::randomLifeTimeIncrementer()
{
  m_meanIncrementer=0.01;
  m_randomIncrementer=((float(rand()) / float(RAND_MAX)) * (0.005f - -0.005f)) + -0.005f;
  m_initialIncrementer=(m_meanIncrementer+(m_randomIncrementer));

  return m_initialIncrementer;
}

//initial speed is randomised and returned to be used in translation calculations
Vec4 pUpdate::initialVelocity()
{
  m_randomSpeedX=((float(rand()) / float(RAND_MAX)) * (0.20f - -0.20f)) + -0.20f;
  m_randomSpeedY=((float(rand()) / float(RAND_MAX)) * (0.20f - -0.20f)) + -0.20f;
  m_velocity.m_x=m_randomSpeedX;
  m_velocity.m_y=m_randomSpeedY;

  return m_velocity;
}

//returns the randomvelocity x and y values to be used in translation calculations
//each particle will vary by a maximum of 2.0f
Vec4 pUpdate::randomVelocity()
{
  m_randomVelocityX=((float(rand()) / float(RAND_MAX)) * (2.0f - -2.0f)) + -2.0f;
  m_randomVelocityY=((float(rand()) / float(RAND_MAX)) * (2.0f - -2.0f)) + -2.0f;
  m_randVelocity.m_x=m_randomVelocityX;
  m_randVelocity.m_y=m_randomVelocityY;

  return m_randVelocity;
}

//returns a random number which will be used in the transparency calculations when a particle first begins to die
float pUpdate::lowerTransparency()
{
  m_randomTransparency=((float(rand()) / float(RAND_MAX)) * (0.0f - 0.5f)) + 0.5f;

  return m_randomTransparency;
}
