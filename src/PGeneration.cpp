///
///  @file PGeneration.cpp
///  @brief where the draw function takes place and where all the parameters to do with the the draw function
///  get passed in, also where the number of particles is generated each time

#include <SDL.h>
#ifdef LINUX
	#include <GL/gl.h>
#endif
#ifdef DARWIN
	#include <OpenGL/gl.h>
#endif

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include "GLFunctions.h"
#include "PAttributes.h"
#include "PDynamics.h"
#include "PGeneration.h"
#include "PUpdate.h"
#include "Vec4.h"

pGeneration::pGeneration()
{
  m_meanParticles=0.0;
  m_variance=0.0;
  m_random=0.0;
  m_particleNumber=0.0;
}

//draw function for the particles, all values passed in from main and each particle is drawn seperately
void pGeneration::draw(const float _r,
                       const float _g,
                       const float _b,
                       const float _a,
                       const float _x,
                       const float _y,
                       const float _z,
                       const float _mousePositionX,
                       const float _mousePositionY,
                       const float _initialVelocityX,
                       const float _initialVelocityY,
                       Vec4 _gravity,
                       float _sizeChange,
                       Vec4 _speedChange,
                       const Vec4 _colourChange,
                       const float _gameTimer,
                       const float _randomVarianceGravity
                      ) const
{
  pAttributes Particle;

  //colour attributes
  float _changeR=_colourChange.m_r;
  float _changeG=_colourChange.m_g;
  float _changeB=_colourChange.m_b;
  //colour includes the change in colour results as particles age
  float red=_r*_changeR;
  float green=_g*_changeG;
  float blue=_b*_changeB;

  //if time hasnt been reached dont implement size, speed and colour changes
  if(_gameTimer<=30)
  {
    _sizeChange=1.0;
    _speedChange.m_x=0.0;
    _speedChange.m_y=0.0;
    _changeR=1.0;
    _changeG=1.0;
    _changeB=1.0;
  }
  //gradually start speed and size change implementation after certain game time
  //after time the full changes effect the particles
  else if(_gameTimer>31 && _gameTimer<=50 && _sizeChange>=(1/0.7))
  {
    _sizeChange=_sizeChange*0.7;
  }
  else if(_gameTimer>31 && _gameTimer<=50)
  {
    _changeR=_changeR*0.7;
    _changeG=_changeG*0.7;
    _changeB=_changeB*0.7;
  }

  //colour limits checking so no colour calculation goes below 0 or above 1
  if(red<=0)
  {
    red=0.1;
  }
  else if(red>=1)
  {
    red=1.0;
  }
  if(green<=0)
  {
    green=0.1;
  }
  else if(green>=1)
  {
    green=1.0;
  }
  if(blue<=0)
  {
    blue=0.1;
  }
  else if(blue>=1)
  {
    blue=1.0;
  }

  Vec4 colour(red, green, blue, Particle.randomTransparency()-_a);

  //affects the y coordinates of gravity by randomising the rate at which the particles fall
  _gravity.m_y=_gravity.m_y*_randomVarianceGravity;

  glPushMatrix();
    colour.colourGL();
    glTranslatef((_mousePositionX*50+(_x))+(_initialVelocityX*0.6)+(_speedChange.m_x),
                 (_mousePositionY*50+(_y))+(_initialVelocityY*0.6)+(_gravity.m_y)+(_speedChange.m_y),
                 _z+30);
    GLFunctions::sphere(Particle.randomSize()*_sizeChange,1);
  glPopMatrix();
}

//assigns the value passed in from main to classes private attribute
void pGeneration::setMeanParticle(const int _mean)
{
  m_meanParticles=_mean;
}

//function that returns the mean number of particles value
Vec4 pGeneration::getMeanParticle() const
{
  return m_meanParticles;
}

//assigns the value passed in from main to classes private attribute
void pGeneration::setVarianceParticle(const int _var)
{
  m_variance=_var;
}

//function that returns the variance value
Vec4 pGeneration::getVarianceParticle() const
{
  return m_variance;
}

//generates a random number of particles (within the parameters) every frame
//random variable will always be a float result between -1.0 and 1.0, the result is converted to
//an integer for the particleNumber so that it can be used to create the particles
float pGeneration::numberOfParticles()
{
  getMeanParticle();
  getVarianceParticle();
  m_random=((float(rand()) / float(RAND_MAX)) * (1.0f - -1.0f)) + -1.0f;
  m_particleNumber=(m_meanParticles+(m_random*m_variance));

  return m_particleNumber;
}
