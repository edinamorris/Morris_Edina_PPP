/// \file PDynamics.h
/// \brief contains functions that affect the dynamics of the particles and the changes to them after a certain time has ellapsed
/// \author Edina Morris
/// \date 30/04/15 Updated

#ifndef PDYNAMICS_H
#define PDYNAMICS_H

#include<iostream>
#include <vector>
#include "Vec4.h"

class pDynamics
{
public:

  pDynamics();

  //timer function called immediately in main and increments values so after a certain game time size, colour and
  //velocity will change, gravity will be used from the moment the particles are created
  void timer();
  void setTimer(const float _timer);
  float getTimer() const;
  void setGravity(const float _gravity);
  float randomiseGravity();
  float sizeChange();
  float randomVarianceGravity();
  Vec4 gravity();
  Vec4 velocityChange();
  Vec4 colourChange();

private:
  float m_gameTimer;
  float m_sizeChange;
  float m_randomVarianceGrav;
  Vec4 m_gravity;
  Vec4 m_velocityChange;
  Vec4 m_colourChange;
};

#endif // PDYNAMICS_H
