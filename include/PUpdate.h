/// \file PUpdate.h
/// \brief contains functions that affect the particles immediately after they are created such as the movement
/// \author Edina Morris
/// \date 30/04/15 Updated

#ifndef PUPDATE_H
#define PUPDATE_H

#include<iostream>
#include <vector>
#include "Vec4.h"

class pUpdate
{
public: 
  pUpdate();

  Vec4 initialVelocity();
  Vec4 randomVelocity();
  float lowerTransparency();
  float particleLifeTime(float killTime);
  float randomLifeTimeIncrementer();

private:
  float m_randomTransparency;
  float m_randomSpeedX;
  float m_randomSpeedY;
  float m_randomVelocityX;
  float m_randomVelocityY;
  Vec4 m_velocity;
  Vec4 m_randVelocity;
  float m_meanLifeTime;
  float m_randomLifeTime;
  float m_initialLifeTime;
  float m_meanIncrementer;
  float m_randomIncrementer;
  float m_initialIncrementer;
};

#endif // PUPDATE_H
