/// \file Matrix3.hpp
/// \brief Declaration of Matrix3 class and any associated global functions.
/// \author Gary M. Zoppetti, Ph.D. & Chad Hogg
/// \version A05

#ifndef MATRIX3_HPP
#define MATRIX3_HPP

// For overload of shift operator
#include <iostream>

#include "Vector3.hpp"

/// \brief A 3x3 matrix of floats.
/// Basis vectors (right, up, and back) are stored in Vector3s and form the
///   columns of a 3x3 matrix.
/// The matrix is interpreted thus:
/// [ rx ux bx ]
/// [ ry uy by ]
/// [ rz uz bz ]
/// Operations are consistent with column vectors (v' = M * v).
class Matrix3
{
public:
  
  /// \brief Initializes a new matrix to the identity matrix.
  /// \post rx, uy, and bz are 1.0f while all other elements are 0.0f.
  Matrix3 ();

  /// \brief Initializes a new matrix from its 9 elements.
  /// \param[in] rx The first column, first row.
  /// \param[in] ry The first column, second row.
  /// \param[in] rz The first column, third row.
  /// \param[in] ux The second column, first row.
  /// \param[in] uy The second column, second row.
  /// \param[in] uz The second column, third row.
  /// \param[in] bx The third column, first row.
  /// \param[in] by The third column, second row.
  /// \param[in] bz The third column, third row.
  /// \post Each element has the value of its matching parameter.
  Matrix3 (float rx, float ry, float rz,
           float ux, float uy, float uz,
           float bx, float by, float bz);
  
  /// \brief Initializes a new matrix from three basis vectors.
  /// \param[in] right The first column of the matrix.
  /// \param[in] up The second column of the matrix.
  /// \param[in] back The third column of the matrix.
  /// \post Each column vector has the value of its matching parameter.
  Matrix3 (const Vector3& right, const Vector3& up,
           const Vector3& back);
  
  /// \brief Initializes a new matrix from two basic vectors, computing the third.
  /// \param[in] right The first column of the matrix.
  /// \param[in] up The second column of the matrix.
  /// \param[in] makeOrthonormal Whether or not to orthonormalize the vectors.
  /// \post The right and up vectors have the value of their matching parameters.
  /// \post The back vector is the cross product of the right and up vectors.
  /// \post If makeOrthonormal is true, the vectors have been orthonormalized.
  Matrix3 (const Vector3& right, const Vector3& up,
           bool makeOrthonormal = false);

  /// \brief Destructs a matrix.
  virtual ~Matrix3 ();
  
  /// \brief Sets this to the identity matrix.
  /// \post rx, uy, and bz are 1.0f while all other elements are 0.0f.
  void
  setToIdentity ();

  /// \brief Sets this to the zero matrix.
  /// \post All elements are 0.0f.
  void
  setToZero ();

  /// \brief Retrieves a pointer to the first column, first row.
  /// \return A pointer to the beginning of the data.
  /// Because of the way our data is stored, you can use pointer arithmetic to
  ///   get to the first column second row, first column third row, second
  ///   column first row, and so forth.
  float*
  data ();

  /// \brief Retrieves a constant pointer to the first column, first row.
  /// \return A constant pointer to the beginning of the data.
  /// Because of the way our data is stored, you can use pointer arithmetic to
  ///   get to the first column second row, first column third row, second
  ///   column first row, and so forth.  
  const float*
  data () const;

  /// \brief Sets the right vector.
  /// \param[in] right The new value for the first column.
  /// \post The first column is a copy of the parameter.
  void
  setRight (const Vector3& right);

  /// \brief Gets the right vector.
  /// \return A copy of the first column.
  Vector3
  getRight () const;

  /// \brief Sets the up vector.
  /// \param[in] up The new value for the second column.
  /// \post The second column is a copy of the parameter.
  void
  setUp (const Vector3& up);

  /// \brief Gets the up vector.
  /// \return A copy of the second column.
  Vector3
  getUp () const;

  /// \brief Sets the back vector.
  /// \param[in] back The new value for the third column.
  /// \post The third column is a copy of the parameter.
  void
  setBack (const Vector3& back);

  /// \brief Gets the back vector.
  /// \return A copy of the third column.
  Vector3
  getBack () const;

  /// \brief Sets the forward (opposite of back) vector.
  /// \param[in] forward The new forward vector.
  /// \post The third column is the negation of the parameter.
  void
  setForward (const Vector3& forward);

  /// \brief Gets the forward (opposite of back) vector.
  /// \return A copy of the negation of the third column.
  Vector3
  getForward () const;

  /// \brief Inverts this matrix, using an fast algorithm that will only work for rotations.
  /// \pre This matrix represents a pure rotation.
  /// \post This matrix has been replaced with its inversion.
  void
  invertRotation ();

  /// \brief Inverts this matrix, using an expensive algorithm.
  /// \post This matrix has been replaced with its inversion.
  void
  invert ();

  /// \brief Calculates the determinant of this matrix.
  /// \return The determinant.
  float
  determinant () const;

  /// \brief Transposes this matrix.
  /// \post The first column has become the first row, etc.
  void
  transpose ();

  /// \brief Makes the basis vectors orthonormal to each other.
  /// Starts by assuming that the back vector is pointing in the correct
  ///   direction.  Normalizes the back vector.  Computes the right vector by
  ///   crossing up with back, and then normalizes it.  Computes the up vector
  ///   by crossing back with right, and then normalizes it.
  /// \post All columns have a length of 1 and are mutually perpendicular.
  /// \post The back vector points in the same direction that it did before.
  void
  orthonormalize ();

  /// \brief Makes this into a uniform scale matrix.
  /// \param[in] scale The amount to scale up (or down).
  /// \post This is a matrix that scales vectors by the specified factor.
  void
  setToScale (float scale);

  /// \brief Makes this into a non-uniform scale matrix.
  /// \param[in] scaleX The scale factor for the X direction.
  /// \param[in] scaleY The scale factor for the Y direction.
  /// \param[in] scaleZ The scale factor for the Z direction.
  /// \post This is a matrix that scales vectors by the specified factors.
  void
  setToScale (float scaleX, float scaleY, float scaleZ);

  /// \brief Makes this into a matrix that shears X values.
  /// \param[in] shearY The amount to shear by Y.
  /// \param[in] shearZ The amount to shear by Z.
  /// \post This is a matrix that shears X by the specified factors of Y and Z.
  void
  setToShearXByYz (float shearY, float shearZ);

  /// \brief Makes this into a matrix that shears Y values.
  /// \param[in] shearX The amount to shear by X.
  /// \param[in] shearZ The amount to shear by Z.
  /// \post This is a matrix that shears Y by the specified factors of X and Z.
  void
  setToShearYByXz (float shearX, float shearZ);

  /// \brief Makes this into a matrix that shears Z values.
  /// \param[in] shearX The amount to shear by X.
  /// \param[in] shearY The amount to shear by Y.
  /// \post This is a matrix that shears Z by the specified factors of X and Y.
  void
  setToShearZByXy (float shearX, float shearY);

  /// \brief Makes this into a matrix that rotates around the X-axis.
  /// \param[in] angleDegrees How much to rotate.
  /// \post This is a matrix that rotates around the X-axis by the specified angle.
  void
  setToRotationX (float angleDegrees);

  /// \brief Makes this into a matrix that rotates around the Y-axis.
  /// \param[in] angleDegrees How much to rotate.
  /// \post This is a matrix that rotates around the Y-axis by the specified angle.
  void
  setToRotationY (float angleDegrees);

  /// \brief Makes this into a matrix that rotates around the Z-axis.
  /// \param[in] angleDegrees How much to rotate.
  /// \post This is a matrix that roates around the Z-axis by the specified angle.
  void
  setToRotationZ (float angleDegrees);

  /// \brief Makes this into a matrix that rotates around an arbitrary vector.
  /// \param[in] angleDegrees How much to rotate.
  /// \param[in] axis The vector to rotate around.
  /// \post This is a matrix that rotates around the specified vector by the specified angle.
  void
  setFromAngleAxis (float angleDegrees, const Vector3& axis);

  /// \brief Negates this matrix.
  /// \post Every element has been replaced by its negation.
  void
  negate ();

  /// \brief Transforms a vector, computing *this * v.
  /// \param[in] v The vector to multiply by this matrix.
  /// \return The result of the multiplication.
  Vector3
  transform (const Vector3& v) const;

  /// \brief Adds another matrix to this.
  /// \param[in] m The other matrix.
  /// \return This matrix.
  /// \post Every element of this matrix has its sum with the equivalent element in the other.
  Matrix3&
  operator+= (const Matrix3& m);

  /// \brief Subtracts another matrix from this.
  /// \param[in] m The other matrix.
  /// \return This matrix.
  /// \post Every element of this matrix has the difference of it and the equivalent element in the other.
  Matrix3&
  operator-= (const Matrix3& m);

  /// \brief Multiplies this matrix by a scalar.
  /// \param[in] scalar The number to multiply by.
  /// \return This matrix.
  /// \post Every element of this matrix has the product of it and the scalar.
  Matrix3&
  operator*= (float scalar);

  /// \brief Multiplies this matrix by another matrix.
  /// \param[in] m The matrix to multiply by.
  /// \return This matrix.
  /// \post This matrix contains the product of itself with m.
  Matrix3&
  operator*= (const Matrix3& m);

private:
  /// \brief The first column of the matrix.
  Vector3 m_right;
  /// \brief The second column of the matrix.
  Vector3 m_up;
  /// \brief The third column of the matrix.
  Vector3 m_back;
};

/// \brief Adds two matrices.
/// \param[in] m1 The first matrix to add.
/// \param[in] m2 The secondn matrix to add.
/// \return A new matrix that is m1 + m2.
Matrix3
operator+ (const Matrix3& m1, const Matrix3& m2);

/// \brief Subtracts two matrices.
/// \param[in] m1 The matrix to subtract from.
/// \param[in] m2 The matrix to subtract.
/// \return A new matrix that is m1 - m2.
Matrix3
operator- (const Matrix3& m1, const Matrix3& m2);

/// \brief Negates a matrix.
/// \param[in] m The matrix to negate.
/// \return A new matrix that is -m.
Matrix3
operator- (const Matrix3& m);

/// \brief Multiplies a matrix by a scalar.
/// \param[in] m The matrix to multiply.
/// \param[in] scalar The number to multiply it by.
/// \return A new matrix that is m * scalar.
Matrix3
operator* (const Matrix3& m, float scalar);

/// \brief Multiplies a matrix by a scalar.
/// \param[in] scalar The number to multiply it by.
/// \param[in] m The matrix to multiply.
/// \return A new matrix that is m * scalar.
Matrix3
operator* (float scalar, const Matrix3& m);

/// \brief Multiplies a matrix by another matrix.
/// \param[in] m1 A matrix.
/// \param[in] m2 Another matrix.
/// \return A new matrix rhat is m * m.
Matrix3
operator* (const Matrix3& m1, const Matrix3& m2);

/// \brief Multiplies a matrix by a vector.
/// \param[in] m A matrix.
/// \param[in] v A vector.
/// \return A new vector that is m * v.
Vector3
operator* (const Matrix3& m, const Vector3& v);

/// \brief Inserts a matrix into an output stream.
/// Each element of the matrix should have 2 digits of precision and a field
///   width of 10.  Elements should be in this order:
///      rx ux bx
///      ry uy by
///      rz uz bz
/// \param[inout] out An output stream.
/// \param[in] m A matrix.
/// \return The output stream.
/// \post The matrix has been inserted into the output stream.
std::ostream&
operator<< (std::ostream& out, const Matrix3& m);

/// \brief Checks whether or not two matrices are equal.
/// matrices are equal if each of their respective elements are within
///   0.00001f of each other due to floating-point imprecision.
/// \param[in] m1 A matrix.
/// \param[in] m2 Another matrix.
/// \return Whether or not m1 and m2 are equal.
bool
operator== (const Matrix3& m1, const Matrix3& m2);


#endif//MATRIX3_HPP
