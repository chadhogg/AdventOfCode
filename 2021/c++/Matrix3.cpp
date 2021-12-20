/// \file Matrix3.cpp
/// \brief Definitions of Matrix3 class member and associated global functions.
/// \author Chad Hogg
/// \version A05

#include <cmath>
#include <iomanip>

#include "Matrix3.hpp"

Matrix3::Matrix3 ()
{
  setToIdentity ();
}

Matrix3::Matrix3 (float rx, float ry, float rz,
		  float ux, float uy, float uz,
		  float bx, float by, float bz)
{
  setRight (Vector3 (rx, ry, rz));
  setUp (Vector3 (ux, uy, uz));
  setBack (Vector3 (bx, by, bz));
}

Matrix3::Matrix3 (const Vector3& right, const Vector3& up,
		  const Vector3& back)
{
  setRight (right);
  setUp (up);
  setBack (back);
}

Matrix3::Matrix3 (const Vector3& right, const Vector3& up,
		  bool makeOrthonormal)
{
  setRight (right);
  setUp (up);
  setBack (right.cross (up));
  if (makeOrthonormal) {
    orthonormalize ();
  }
}

Matrix3::~Matrix3 ()
{
}

void
Matrix3::setToIdentity ()
{
  setRight (Vector3 (1.0f, 0.0f, 0.0f));
  setUp (Vector3 (0.0f, 1.0f, 0.0f));
  setBack (Vector3 (0.0f, 0.0f, 1.0f));
}

void
Matrix3::setToZero ()
{
  setRight (Vector3 (0.0f, 0.0f, 0.0f));
  setUp (Vector3 (0.0f, 0.0f, 0.0f));
  setBack (Vector3 (0.0f, 0.0f, 0.0f));
}

float*
Matrix3::data ()
{
  return &m_right.m_x;
}

const float*
Matrix3::data () const
{
  return &m_right.m_x;
}

void
Matrix3::setRight (const Vector3& right)
{
  m_right = right;
}

Vector3
Matrix3::getRight () const
{
  return m_right;
}

void
Matrix3::setUp (const Vector3& up)
{
  m_up = up;
}

Vector3
Matrix3::getUp () const
{
  return m_up;
}

void
Matrix3::setBack (const Vector3& back)
{
  m_back = back;
}

Vector3
Matrix3::getBack () const
{
  return m_back;
}

void
Matrix3::setForward (const Vector3& forward)
{
  m_back = -forward;
}

Vector3
Matrix3::getForward () const
{
  return -m_back;
}

void
Matrix3::invertRotation ()
{
  transpose ();
}

// Uses formula from Lecture 05, slide 12
void
Matrix3::invert ()
{
  float scale = 1.0f / determinant ();
  Vector3 newRight ( scale * ( m_up.m_y * m_back.m_z - m_back.m_y * m_up.m_z),
		     scale * ( m_back.m_y * m_right.m_z - m_right.m_y * m_back.m_z),
		     scale * ( m_right.m_y * m_up.m_z - m_up.m_y * m_right.m_z));
  Vector3 newUp ( scale * ( m_back.m_x * m_up.m_z - m_up.m_x * m_back.m_z),
		  scale * ( m_right.m_x * m_back.m_z - m_back.m_x * m_right.m_z),
		  scale * ( m_up.m_x * m_right.m_z - m_right.m_x * m_up.m_z));
  Vector3 newBack ( scale * ( m_up.m_x * m_back.m_y - m_back.m_x * m_up.m_y),
		    scale * ( m_back.m_x * m_right.m_y - m_right.m_x * m_back.m_y),
		    scale * ( m_right.m_x * m_up.m_y - m_up.m_x * m_right.m_y));
  setRight (newRight);
  setUp (newUp);
  setBack (newBack);
}

float
Matrix3::determinant () const
{
  float det = 0.0f
    + m_right.m_x * (m_up.m_y * m_back.m_z - m_back.m_y * m_up.m_z)
    - m_right.m_y * (m_up.m_x * m_back.m_z - m_back.m_x * m_up.m_z)
    + m_right.m_z * (m_up.m_x * m_back.m_y - m_back.m_x * m_up.m_y);
  return det;
}

void
Matrix3::transpose ()
{
  Vector3 newRight (m_right.m_x, m_up.m_x, m_back.m_x);
  Vector3 newUp (m_right.m_y, m_up.m_y, m_back.m_y);
  Vector3 newBack (m_right.m_z, m_up.m_z, m_back.m_z);
  setRight (newRight);
  setUp (newUp);
  setBack (newBack);
}

void
Matrix3::orthonormalize ()
{
  m_back.normalize ();
  m_right = m_up.cross (m_back);
  m_right.normalize ();
  m_up = m_back.cross (m_right);
}

void
Matrix3::setToScale (float scale)
{
  setToScale (scale, scale, scale);
}

void
Matrix3::setToScale (float scaleX, float scaleY, float scaleZ)
{
  Vector3 newRight (scaleX, 0.0f, 0.0f);
  Vector3 newUp (0.0f, scaleY, 0.0f);
  Vector3 newBack (0.0f, 0.0f, scaleZ);
  setRight (newRight);
  setUp (newUp);
  setBack (newBack);  
}

void
Matrix3::setToShearXByYz (float shearY, float shearZ)
{
  setToIdentity ();
  m_up.m_x = shearY;
  m_back.m_x = shearZ;
}

void
Matrix3::setToShearYByXz (float shearX, float shearZ)
{
  setToIdentity ();
  m_right.m_y = shearX;
  m_back.m_y = shearZ;
}

void
Matrix3::setToShearZByXy (float shearX, float shearY)
{
  setToIdentity ();
  m_right.m_z = shearX;
  m_up.m_z = shearY;
}

void
Matrix3::setToRotationX (float angleDegrees)
{
  float radians = angleDegrees * M_PI / 180.0f;
  float sine = sin (radians);
  float cosine = cos (radians);
  m_right = Vector3 (1.0f, 0.0f, 0.0f);
  m_up = Vector3 (0.0f, cosine, sine);
  m_back = Vector3 (0.0f, -sine, cosine);
}

void
Matrix3::setToRotationY (float angleDegrees)
{
  float radians = angleDegrees * M_PI / 180.0f;
  float sine = sin (radians);
  float cosine = cos (radians);
  m_right = Vector3 (cosine, 0.0f, -sine);
  m_up = Vector3 (0.0f, 1.0f, 0.0f);
  m_back = Vector3 (sine, 0.0f, cosine);
}

void
Matrix3::setToRotationZ (float angleDegrees)
{
  float radians = angleDegrees * M_PI / 180.0f;
  float sine = sin (radians);
  float cosine = cos (radians);
  m_right = Vector3 (cosine, sine, 0.0f);
  m_up = Vector3 (-sine, cosine, 0.0f);
  m_back = Vector3 (0.0f, 0.0f, 1.0f);
}

// From https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
void
Matrix3::setFromAngleAxis (float angleDegrees, const Vector3& axis)
{
  Vector3 norm = axis;
  norm.normalize ();
  float radians = angleDegrees * M_PI / 180.0f;
  float sine = sin (radians);
  float cosine = cos (radians);
  m_right = Vector3 (cosine + norm.m_x * norm.m_x * (1 - cosine),
		     norm.m_y * norm.m_x * (1 - cosine) + norm.m_z * sine,
		     norm.m_z * norm.m_x * (1 - cosine) - norm.m_y * sine);
  m_up = Vector3 (norm.m_x * norm.m_y * (1 - cosine) - norm.m_z * sine,
		  cosine + norm.m_y * norm.m_y * (1 - cosine),
		  norm.m_z * norm.m_y * (1 - cosine) + norm.m_x * sine);
  m_back = Vector3 (norm.m_x * norm.m_z * (1 - cosine) + norm.m_y * sine,
		    norm.m_y * norm.m_z * (1 - cosine) - norm.m_x * sine,
		    cosine + norm.m_z * norm.m_z * (1 - cosine));
}

void
Matrix3::negate ()
{
  m_right *= -1.0f;
  m_up *= -1.0f;
  m_back *= -1.0f;
}

Vector3
Matrix3::transform (const Vector3& v) const
{
  Vector3 result (m_right.m_x * v.m_x + m_up.m_x * v.m_y + m_back.m_x * v.m_z,
		  m_right.m_y * v.m_x + m_up.m_y * v.m_y + m_back.m_y * v.m_z,
		  m_right.m_z * v.m_x + m_up.m_z * v.m_y + m_back.m_z * v.m_z);
  return result;
}

Matrix3&
Matrix3::operator+= (const Matrix3& m)
{
  m_right += m.m_right;
  m_up += m.m_up;
  m_back += m.m_back;
  return *this;
}

Matrix3&
Matrix3::operator -= (const Matrix3& m)
{
  m_right -= m.m_right;
  m_up -= m.m_up;
  m_back -= m.m_back;
  return *this;
}

Matrix3&
Matrix3::operator*= (float scalar)
{
  m_right *= scalar;
  m_up *= scalar;
  m_back *= scalar;
  return *this;
}

Matrix3&
Matrix3::operator*= (const Matrix3& m)
{
  Matrix3 t(*this);
  t.transpose ();
  setRight (Vector3 (t.getRight ().dot (m.getRight ()),
		     t.getUp ().dot (m.getRight ()),
		     t.getBack ().dot (m.getRight ())));
  setUp ( Vector3 (t.getRight ().dot (m.getUp ()),
		   t.getUp ().dot (m.getUp ()),
		   t.getBack ().dot (m.getUp ())));
  setBack ( Vector3 (t.getRight ().dot (m.getBack ()),
		     t.getUp ().dot (m.getBack ()),
		     t.getBack ().dot (m.getBack ())));
  return *this;
}

Matrix3
operator+ (const Matrix3& m1, const Matrix3& m2)
{
  Matrix3 p = m1;
  p += m2;
  return p;
}

Matrix3
operator- (const Matrix3& m1, const Matrix3& m2)
{
  Matrix3 p = m1;
  p -= m2;
  return p;
}

Matrix3
operator- (const Matrix3& m)
{
  return m * -1.0f;
}

Matrix3
operator* (const Matrix3& m, float scalar)
{
  Matrix3 p = m;
  p *= scalar;
  return p;
}

Matrix3
operator* (float scalar, const Matrix3& m)
{
  return m * scalar;
}

Matrix3
operator* (const Matrix3& m1, const Matrix3 & m2)
{
  Matrix3 p = m1;
  p *= m2;
  return p;
}

Vector3
operator* (const Matrix3& m, const Vector3& v)
{
  return m.transform (v);
}

std::ostream&
operator<< (std::ostream& out, const Matrix3& m)
{
  Vector3 right = m.getRight ();
  Vector3 up = m.getUp ();
  Vector3 back = m.getBack ();
  out << std::fixed;
  out << std::setprecision(2) << std::setw(10) << right.m_x << std::setw(10) << up.m_x << std::setw(10) << back.m_x << std::endl;
  out << std::setprecision(2) << std::setw(10) << right.m_y << std::setw(10) << up.m_y << std::setw(10) << back.m_y << std::endl;
  out << std::setprecision(2) << std::setw(10) << right.m_z << std::setw(10) << up.m_z << std::setw(10) << back.m_z << std::endl;
  return out;
}

bool
operator== (const Matrix3& m1, const Matrix3& m2)
{
  return
    m1.getRight () == m2.getRight () &&
    m1.getUp () == m2.getUp () &&
    m1.getBack () == m2.getBack ();
}
