/// \file Vector3.cpp
/// \brief Definitions of Vector3 class member and associated global functions.
/// \author Chad Hogg
/// \version A04

#include <iostream>
#include <iomanip>
#include <cmath>

#include "Vector3.hpp"


Vector3::Vector3 ()
{
  set (0.0f);
}

Vector3::Vector3 (float xyz)
{
  set (xyz);
}

Vector3::Vector3 (float x, float y, float z)
{
  set (x, y, z);
}

void
Vector3::set (float xyz)
{
  set (xyz, xyz, xyz);
}

void
Vector3::set (float x, float y, float z)
{
  m_x = x;
  m_y = y;
  m_z = z;
}

void
Vector3::negate ()
{
  m_x = -m_x;
  m_y = -m_y;
  m_z = -m_z;
}

float
Vector3::dot (const Vector3& v) const
{
  return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z;
}

float
Vector3::angleBetween (const Vector3& v) const
{
  return acos (dot (v) / (length () * v.length ()));
}

Vector3
Vector3::cross (const Vector3& v) const
{
  return Vector3 (m_y * v.m_z - m_z * v.m_y,
		  m_z * v.m_x - m_x * v.m_z,
		  m_x * v.m_y - m_y * v.m_x);
}

float
Vector3::length () const
{
  return sqrt (m_x * m_x + m_y * m_y + m_z * m_z);
}

void
Vector3::normalize ()
{
  *this /= length ();
}

Vector3&
Vector3::operator+= (const Vector3& v)
{
  m_x += v.m_x;
  m_y += v.m_y;
  m_z += v.m_z;
  return *this;
}

Vector3&
Vector3::operator-= (const Vector3& v)
{
  m_x -= v.m_x;
  m_y -= v.m_y;
  m_z -= v.m_z;
  return *this;
}

Vector3&
Vector3::operator*= (float s)
{
  m_x *= s;
  m_y *= s;
  m_z *= s;
  return *this;
}

Vector3&
Vector3::operator/= (float s)
{
  m_x /= s;
  m_y /= s;
  m_z /= s;
  return *this;
}


Vector3
operator+ (const Vector3& v1, const Vector3& v2)
{
  Vector3 r(v1);
  r += v2;
  return r;
}

Vector3
operator- (const Vector3& v1, const Vector3& v2)
{
  Vector3 r(v1);
  r -= v2;
  return r;
}

Vector3
operator- (const Vector3& v)
{
  Vector3 r(v);
  r.negate ();
  return r;
}

Vector3
operator* (float s, const Vector3& v)
{
  Vector3 r(v);
  r *= s;
  return r;
}

Vector3
operator* (const Vector3& v, float s)
{
  return s * v;
}

Vector3
operator/ (const Vector3& v, float s)
{
  Vector3 r(v);
  r /= s;
  return r;
}

std::ostream&
operator<< (std::ostream& out, const Vector3& v)
{
  out << std::fixed
      << std::setw (10) << std::setprecision (2) << v.m_x
      << std::setw (10) << std::setprecision (2) << v.m_y
      << std::setw (10) << std::setprecision (2) << v.m_z;
  return out;
}

bool
operator== (const Vector3& v1, const Vector3& v2)
{
  const float EPSILON = 0.00001f;
  return fabs (v1.m_x - v2.m_x) < EPSILON &&
	 fabs (v1.m_y - v2.m_y) < EPSILON &&
	 fabs (v1.m_z - v2.m_z) < EPSILON;
}
