/// \file Vector3.hpp
/// \brief Declaration of Vector class and any associated global functions.
/// \author Chad Hogg
/// \version A04

#ifndef VECTOR3_HPP
#define VECTOR3_HPP

// For overload of shift operator
#include <iostream>

/// \brief A vector of 3 floating-point numbers.
/// These should behave just like our normal mathematical understanding of
///   vectors.
/// We represent the vector as a linear combination of three basis vectors,
///   storing the coefficients of each.
/// They have public data members because any combination of x, y, and z
///   values is a legal vector, and because we would like this class to be
///   easy and efficient to use.
class Vector3
{
 public:

  /// \brief Initializes a new vector to have all coefficients 0.0f.
  /// \post All coefficients are 0.0f.
  Vector3 ();

  /// \brief Initializes a new vector to have all coefficients identical.
  /// \param[in] xyz The value that should be used for all three coefficients.
  /// \post All coefficients are equal to xyz.
  Vector3 (float xyz);

  /// \brief Initializes a new vector with custom coefficients.
  /// \param[in] x The coefficient for the basis vector i.
  /// \param[in] y The coefficient for the basis vector j.
  /// \param[in] z The coefficient for the basis vector k.
  /// \post The coefficients are equal to x, y, and z respectively.
  Vector3 (float x, float y, float z);

  /// \brief Sets each coefficient to the same value.
  /// \param[in] xyz The value that should be used for all three coefficients.
  /// \post All coefficients are equal to xyz.
  void
  set (float xyz);

  /// \brief Sets each coefficient to (potentially) different values.
  /// \param[in] x The new coefficient for the basis vector i.
  /// \param[in] y The new coefficient for the basis vector j.
  /// \param[in] z The new coefficient for the basis vector k.
  /// \post The coefficients are equal to x, y, and z respectively.
  void
  set (float x, float y, float z);

  /// \brief Replaces the direction of this vector to its exact opposite.
  /// \post The vector has been negated.
  void
  negate (); 

  /// \brief Compute the dot product of this with another vector.
  /// \param[in] v The other vector.
  /// \return The dot product of this and v.
  float
  dot (const Vector3& v) const;

  /// \brief Computes the angle (in radians) between this and another vector.
  /// \param[in] v The other vector.
  /// \return The angle between this and v, expressed in radians.
  float
  angleBetween (const Vector3& v) const;

  /// \brief Computes the cross product between this and another vector.
  /// \param[in] v The other vector.
  /// \return The cross product of this vector with v.
  Vector3
  cross (const Vector3& v) const;

  /// \brief Computes the length of this vector.
  /// \return The length of this vector.
  float
  length () const;

  /// \brief Normalizes this vector.
  /// \post This vector points in the same direction, but has a length of 1.
  void
  normalize ();

  /// \brief Adds another vector to this one.
  /// \param[in] v Another vector.
  /// \post This vector has been replaced by itself plus v.
  /// \return This vector.
  Vector3&
  operator+= (const Vector3& v);

  /// \brief Subtracts another vector from this one.
  /// \param[in] v Another vector.
  /// \post This vector has been replaced by itself minus v.
  /// \return This vector.
  Vector3&
  operator-= (const Vector3& v);

  /// \brief Multiplies this vector by a scalar.
  /// \param[in] s A scalar.
  /// \post This vector has been replaced with itself times s.
  /// \return This vector.
  Vector3&
  operator*= (float s);

  /// \brief Divies this vector by a scalar.
  /// \param[in] s A scalar.
  /// \post This vector has been replaced with itself divided by s.
  /// \return This vector.
  Vector3&
  operator/= (float s);

  /// \brief The coefficient of the basis vector i.
  float m_x;
  /// \brief The coefficient of the basis vector j.
  float m_y;
  /// \brief The coefficient of the basis vector k.
  float m_z;
};

/// \brief Adds two vectors.
/// \param[in] v1 The first addend.
/// \param[in] v2 The second addend.
/// \return A new vector that is v1 + v2.
Vector3
operator+ (const Vector3& v1, const Vector3& v2);

/// \brief Subtracts two vectors.
/// \param[in] v1 The minuend.
/// \param[in] v2 The subtrahend.
/// \return A new vector that is v1 - v2.
Vector3
operator- (const Vector3& v1, const Vector3& v2);

/// \brief Negates a vector.
/// \param[in] v A vector.
/// \return A new vector that is the negation of v.
Vector3
operator- (const Vector3& v);

/// \brief Multiplies a scalar by a vector.
/// \param[in] s A scalar.
/// \param[in] v A vector.
/// \return A new vector that is s * v.
Vector3
operator* (float s, const Vector3& v);

/// \brief Multiplies a vector by a scalar.
/// \param[in] v A vector.
/// \param[in] s A scalar.
/// \return A new vector that is v * s.
Vector3
operator* (const Vector3& v, float s);

/// \brief Divides a vector by a scalar.
/// \param[in] v A vector.
/// \param[in] s A scalar.
/// \return A new vector that is v / s.
Vector3
operator/ (const Vector3& v, float s);

/// \brief Inserts a vector into an output stream.
/// Each component of the vector should have 2 digits of precision and a field
///   width of 10.
/// \param[in] out An output stream.
/// \param[in] v A vector.
/// \return The output stream.
/// \post The vector has been inserted into the output stream.
std::ostream&
operator<< (std::ostream& out, const Vector3& v);

/// \brief Checks whether or not two vectors are equal.
/// Vectors are equal if each of their respective components are within
///   0.00001f of each other due to floating-point imprecision.
/// \param[in] v1 A vector.
/// \param[in] v2 Another vector.
/// \return Whether or not v1 and v2 are equal.
bool
operator== (const Vector3& v1, const Vector3& v2);

#endif//VECTOR3_HPP
