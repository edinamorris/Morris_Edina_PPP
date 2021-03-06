///
///  @file Mat4.cpp
///  @brief contains functions for math calculations

#include "Mat4.h"
#include <cstring>
#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#ifdef LINUX
	#include <GL/gl.h>
#endif
#ifdef DARWIN
	#include <OpenGL/gl.h>
#endif

/**
 * @brief Mat4::Mat4
 * @param _s
 */
Mat4::Mat4(float _s)
{
  memset(&m_m,0,sizeof(m_m));
  m_00=_s;
  m_11=_s;
  m_22=_s;
  m_33=_s;
}

/**
 * @brief Mat4::rotateX
 * @param _deg
 */
void Mat4::rotateX(float _deg)
{
  float beta=(_deg/180.0f) * M_PI;
  float sr= sin(beta);
  float cr= cos(beta);
  identity();
  m_11 = cr;
  m_21 = -sr;
  m_12 = sr;
  m_22 = cr;
}

/**
 * @brief Mat4::rotateY
 * @param _deg
 */
void Mat4::rotateY(float _deg)
{
  float beta=(_deg/180.0f) * M_PI;
  float sr= sin(beta);
  float cr= cos(beta);
  identity();
  m_00 = cr;
  m_20 = sr;
  m_02 = -sr;
  m_22 = cr;
}

/**
 * @brief Mat4::rotateZ
 * @param _deg
 */
void Mat4::rotateZ(float _deg)
{
  float beta=(_deg/180.0f) * M_PI;
  float sr= sin(beta);
  float cr= cos(beta);
  identity();
  m_00 = cr;
  m_10 = -sr;
  m_01 = sr;
  m_11 = cr;
}

/**
 * @brief Mat4::transpose
 */
void Mat4::transpose()
{
  Mat4 copy(*this);

  for(int x=0; x<4; x++)
    {
      for(int y=0; y<4; y++)
        {
          m_m[x][y] = copy.m_m[y][x];
        }
    }

}

/**
 * @brief Mat4::Mat4
 * @param _r
 */
Mat4::Mat4(const Mat4 &_r)
{
  memcpy(m_m,&_r.m_m,sizeof(m_m));
}

/**
 * @brief Mat4::identity
 */
void Mat4::identity()
{
  memset(&m_m,0,sizeof(m_m));
  m_00=1.0f;
  m_11=1.0f;
  m_22=1.0f;
  m_33=1.0f;
}

/**
 * @brief Mat4::loadModelView
 */
void Mat4::loadModelView() const
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMultMatrixf(&m_openGL[0]);
}

/**
 * @brief Mat4::loadProjection
 */
void Mat4::loadProjection() const
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMultMatrixf(&m_openGL[0]);
}

std::ostream & operator <<(std::ostream &_output, const Mat4 &_v)
{
  std::cout.setf(std::ios::fixed |
                std::ios::adjustfield |
                std::ios::showpos
                );
  std::cout.precision(std::numeric_limits<double>::digits10+1);
  return _output
    <<"["<<_v.m_00<<","<<_v.m_10<<","<<_v.m_20<<","<<_v.m_30<<"]\n"
    <<"["<<_v.m_01<<","<<_v.m_11<<","<<_v.m_21<<","<<_v.m_31<<"]\n"
    <<"["<<_v.m_02<<","<<_v.m_12<<","<<_v.m_22<<","<<_v.m_32<<"]\n"
    <<"["<<_v.m_03<<","<<_v.m_13<<","<<_v.m_23<<","<<_v.m_33<<"]\n";
}
