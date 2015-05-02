/// \file PAttributes.h
/// \brief contains functions that generate random values for each particle when they are first created
/// \author Edina Morris
/// \date 30/04/15 Updated

#ifndef PATTRIBUTES_H
#define PATTRIBUTES_H

#include<iostream>
#include <vector>
#include "Vec4.h"

class pAttributes
{
public:
  pAttributes();

  void randomSpeed();
  Vec4 randomColour();
  float randomTransparency();
  void setMeanPosition(const float _x, const float _y, const float _z);
  void setMeanColour(const float _r, const float _g, const float _b);
  Vec4 getMeanColour() const;
  Vec4 getMeanPosition() const;
  Vec4 randomPosition();
  float randomSize();
  void setMousePosition(const float _xMouse, const float _yMouse);
  Vec4 getMousePosition() const;

private:
  float m_initialSize;
  float m_meanSize;
  float m_randomSize;
  float m_meanTransparency;
  float m_randomTransparency;
  float m_initialTransparency;
  float m_mouseX;
  float m_mouseY;
  Vec4 m_coordinates;
  Vec4 m_meanPosition;
  Vec4 m_initialPosition;
  Vec4 m_randomPosition;
  Vec4 m_mouse;
  Vec4 m_randomColour;
  Vec4 m_initialColour;
  Vec4 m_meanColour;
  Vec4 m_initialVelocity;

};

#endif // PATTRIBUTES_H
