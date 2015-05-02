///
///  @file PAttributes.cpp
///  @brief where random parameters are genereated for each individual particle, the parameters here
///  are the fireworks initial attributes when the particles are first drawn on screen

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
#include <GL/glu.h>
#include "GLFunctions.h"
#include "PAttributes.h"

pAttributes::pAttributes()
{
  m_initialSize=0.0;
  m_initialTransparency=0.0;
  m_meanColour.m_x=0.0;
  m_meanColour.m_y=0.0;
  m_meanColour.m_z=0.0;
  m_initialColour.m_x=0.0;
  m_initialColour.m_y=0.0;
  m_initialColour.m_z=0.0;
  m_meanPosition.m_x=0.0;
  m_meanPosition.m_y=0.0;
  m_meanPosition.m_z=0.0;
  m_initialPosition.m_x=0.0;
  m_initialPosition.m_y=0.0;
  m_initialPosition.m_z=0.0;
  m_coordinates.m_x=0.0;
  m_coordinates.m_y=0.0;
  m_coordinates.m_z=0.0;
}

//function to calculate randomsize for each particle based on a mean size and variance
float pAttributes::randomSize()
{
  m_meanSize=0.1;
  m_randomSize=((float(rand()) / float(RAND_MAX)) * (0.05f - -0.05f)) + -0.05f;
  m_initialSize=(m_meanSize+m_randomSize);

  return m_initialSize;
}

//function to set the mean colour which has been passed in from main, the colour can be changed directly from there
void pAttributes::setMeanColour(const float _r,
                                const float _g,
                                const float _b
                               )
{
  m_meanColour.m_r=_r;
  m_meanColour.m_g=_g;
  m_meanColour.m_b=_b;
}

//function to return the meanColour
Vec4 pAttributes::getMeanColour() const
{
  return m_meanColour;
}

//function to create a random transparency for each particle based on a mean and variance value
float pAttributes::randomTransparency()
{
  m_meanTransparency=0.9;
  m_randomTransparency=((float(rand()) / float(RAND_MAX)) * (0.1f - -0.05f)) + -0.05f;
  m_initialTransparency=(m_meanTransparency+(m_randomTransparency));

  //if the transparency happens to be calculated above or below the limits then it is automatically adjusted
  if(m_initialTransparency>=1.0)
  {
    m_initialTransparency=1;
  }
  else if(m_initialTransparency<=0.0)
  {
    m_initialTransparency=0;
  }

  return m_initialTransparency;
}

//function to randomise the colour based on user input in the main function and randomised values
Vec4 pAttributes::randomColour()
{
  getMeanColour();
  m_randomColour.m_r=((float(rand()) / float(RAND_MAX)) * (0.3f - -0.3f)) + -0.3f;
  m_initialColour.m_r=(m_meanColour.m_r+(m_randomColour.m_r));

  m_randomColour.m_g=((float(rand()) / float(RAND_MAX)) * (0.3f - -0.3f)) + -0.3f;
  m_initialColour.m_g=(m_meanColour.m_g+(m_randomColour.m_g));

  m_randomColour.m_b=((float(rand()) / float(RAND_MAX)) * (0.3f - -0.3f)) + -0.3f;
  m_initialColour.m_b=(m_meanColour.m_b+(m_randomColour.m_b));

  //if any of the values go above or below their limits then they are automatically adjusted
  if(m_initialColour.m_r>1||m_initialColour.m_g>1||m_initialColour.m_b>1)
  {
    m_initialColour.m_r=1;
    m_initialColour.m_g=1;
    m_initialColour.m_b=1;
  }
  else if(m_initialColour.m_r<=0||m_initialColour.m_g<=0||m_initialColour.m_b<=0)
  {
    m_initialColour.m_r=0.1;
    m_initialColour.m_g=0.1;
    m_initialColour.m_b=0.1;
  }

  return m_initialColour;
}

//function to set the mean position which is passed in from main
void pAttributes::setMeanPosition(const float _x,
                                  const float _y,
                                  const float _z
                                 )
{
  m_meanPosition.m_x=_x;
  m_meanPosition.m_y=_y;
  m_meanPosition.m_z=_z;
}

//function returns the meanposition
Vec4 pAttributes::getMeanPosition() const
{
  return m_meanPosition;
}

//gets the mean position and calculates an initial position for each particle, x, y and z
//using the meanPosition and a random calculation
Vec4 pAttributes::randomPosition()
{
  getMeanPosition();
  m_randomPosition.m_x=((float(rand()) / float(RAND_MAX)) * (0.25f - -0.25f)) + -0.25f;
  m_initialPosition.m_x=(m_meanPosition.m_x+(m_randomPosition.m_x));

  m_randomPosition.m_y=((float(rand()) / float(RAND_MAX)) * (0.25f - -0.25f)) + -0.25f;
  m_initialPosition.m_y=(m_meanPosition.m_y+(m_randomPosition.m_y));

  m_randomPosition.m_z=((float(rand()) / float(RAND_MAX)) * (0.25f - -0.25f)) + -0.25f;
  m_initialPosition.m_z=(m_meanPosition.m_z+(m_randomPosition.m_z));

  return m_initialPosition;
}

//function used to convert screen coordinates to world coordinates, current mouse screen position is passed in
/// @brief a function that takes in screen coordiantes and calculates object coordinates
/// Modified from :-
/// NeHe productions. Using gluUnProject [online].
/// [Accessed 2015]. Available from: <http://nehe.gamedev.net/article/using_gluunproject/16013/>.
void pAttributes::setMousePosition(const float _xMouse,
                                   const float _yMouse
                                  )
{
  GLint viewport[4];
  GLdouble modelview[16];
  GLdouble projection[16];
  GLfloat xPos=_xMouse, yPos=_yMouse, zPos=0;
  GLdouble objectCoordinates[3];

  //getting matricies
  glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
  glGetDoublev( GL_PROJECTION_MATRIX, projection );
  glGetIntegerv( GL_VIEWPORT, viewport );

  xPos = _xMouse;
  yPos = (float)viewport[3] - _yMouse;

  //unprojects the screen coordinates by undoing the matricies performed on them to get object coordinates
  gluUnProject( xPos, yPos, zPos, modelview, projection, viewport, &objectCoordinates[0], &objectCoordinates[1], &objectCoordinates[2]);

  //assigning the results
  m_coordinates.m_x=objectCoordinates[0];
  m_coordinates.m_y=objectCoordinates[1];
  m_coordinates.m_z=objectCoordinates[2];
}


//function returns the mousePosition which is used generating the particles in the correct place in the world
Vec4 pAttributes::getMousePosition() const
{
  return m_coordinates;
}

