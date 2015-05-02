/// \file PGeneration.h
/// \brief contains functions that draw the particles and generate the random number of particles
/// \author Edina Morris
/// \date 30/04/15 Updated

#ifndef PGENERATION_H
#define PGENERATION_H

#include<iostream>
#include <vector>
#include "Vec4.h"

class pGeneration
{
public:
  pGeneration();

  void draw(const float _r,
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
           ) const;
  float numberOfParticles();
  void setMeanParticle(const int _mean);
  Vec4 getMeanParticle() const;
  void setVarianceParticle(const int _var);
  Vec4 getVarianceParticle() const;

private:
  float m_meanParticles;
  float m_variance;
  float m_random;
  int m_particleNumber;
};

#endif // PGENERATION_H
