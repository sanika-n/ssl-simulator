/***************************************************************************
 *   Copyright 2015 Philipp Nordhus                                        *
 *   Robotics Erlangen e.V.                                                *
 *   http://www.robotics-erlangen.de/                                      *
 *   info@robotics-erlangen.de                                             *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

 #ifndef VECTOR_H
 #define VECTOR_H
 
 #include <cmath>
 #include <ostream>
 
 /*!
  * \ingroup path
  * \brief 2-dimensional vector class
  */
 class Vector
 {
 public:
     /*!
      * \brief Constructor
      *
      * This constructor does not initialize any members
      */
     Vector() {}
 
     /*!
      * \brief Initializing constructor
      * \param x x-coordinate of the vector
      * \param y y-coordinate of the vector
      */
     Vector(float x, float y) { d[0] = x; d[1] = y; }
 
 public:
     /*!
      * \brief Access vector element by index (non-const)
      * \param index 0 for x, 1 for y
      * \return Reference to the value
      */
     float& operator[](unsigned int index);
 
     /*!
      * \brief Access vector element by index (const)
      * \param index 0 for x, 1 for y
      * \return Value at the index
      */
     float operator[](unsigned int index) const;
 
     /*!
      * \brief Vector addition
      * \param rho The vector to add
      * \return Resulting vector
      */
     Vector operator+(const Vector &rho) const;
 
     /*!
      * \brief Vector subtraction
      * \param rho The vector to subtract
      * \return Resulting vector
      */
     Vector operator-(const Vector &rho) const;
 
     /*!
      * \brief Scalar multiplication
      * \param scalar Multiplier
      * \return Resulting vector
      */
     Vector operator*(float scalar) const;
 
     /*!
      * \brief Scalar division
      * \param scalar Divisor
      * \return Resulting vector
      */
     Vector operator/(float scalar) const;
 
     /*!
      * \brief Scalar multiply-assign
      * \param scalar Multiplier
      * \return Reference to this vector
      */
     Vector& operator*=(float scalar);
 
     /*!
      * \brief Vector add-assign
      * \param other Vector to add
      * \return Reference to this vector
      */
     Vector& operator+=(const Vector &other);
 
     /*!
      * \brief Dot product
      * \param rho The other vector
      * \return Scalar dot product
      */
     float operator*(const Vector &rho) const;
 
     /*!
      * \brief Equality check
      * \param rho The other vector
      * \return True if equal
      */
     bool operator==(const Vector &rho) const;
 
     /*!
      * \brief Inequality check
      * \param rho The other vector
      * \return True if not equal
      */
     bool operator!=(const Vector &rho) const;
 
     /*!
      * \brief Calculate a perpendicular vector
      * \return A vector perpendicular to this one (clockwise rotation)
      */
     Vector perpendicular() const;
 
     /*!
      * \brief Normalize the vector
      * \return A normalized copy of the vector
      */
     Vector normalized() const;
 
     /*!
      * \brief Calculate the length of the vector
      * \return The length of the vector
      */
     float length() const;
 
     /*!
      * \brief Calculate the squared length of the vector
      * \return The squared length of the vector
      */
     float lengthSquared() const;
 
     /*!
      * \brief Calculate the distance to another vector
      * \param rho Another vector
      * \return The Euclidean distance to the other vector
      */
     float distance(const Vector &rho) const;
 
     /*!
      * \brief Calculate the squared distance to another vector
      * \param rho Another vector
      * \return The squared Euclidean distance
      */
     float distanceSq(const Vector &rho) const {
         return (*this - rho).lengthSquared();
     }
 
     /*!
      * \brief Dot product function
      * \param other Other vector
      * \return Scalar dot product
      */
     float dot(const Vector &other) const {
         return x * other.x + y * other.y;
     }
 
     /*!
      * \brief Determinant of three points
      * \param a First vector
      * \param b Second vector
      * \param c Third vector
      * \return Determinant value (twice the signed triangle area)
      */
     static float det(const Vector &a, const Vector &b, const Vector &c) {
         return a.x * b.y + b.x * c.y + c.x * a.y - a.x * c.y - b.x * a.y - c.x * b.y;
     }
 
     /*!
      * \brief Angle of the vector
      * \return Angle in radians (clockwise from Y-axis)
      */
     float angle() const {
         return std::atan2(x, y) + float(2 * M_PI);
     }
 
     /*!
      * \brief Absolute value of components
      * \return A vector with absolute x and y values
      */
     Vector abs() const  {
         return {std::abs(x), std::abs(y)};
     }
 
     /*!
      * \brief Output stream operator
      * \param stream Output stream
      * \param v Vector to print
      * \return Reference to the output stream
      */
     friend std::ostream& operator<<(std::ostream &stream, const Vector v);
 
 public:
     union {
         struct {
             float x; //!< X component
             float y; //!< Y component
         };
         float d[2]; //!< Array-based component access
     };
 };
 
 inline float& Vector::operator[](unsigned int index)
 {
     return d[index];
 }
 
 inline float Vector::operator[](unsigned int index) const
 {
     return d[index];
 }
 
 inline Vector Vector::operator+(const Vector &rho) const
 {
     return Vector(x + rho.x, y + rho.y);
 }
 
 inline Vector Vector::operator-(const Vector &rho) const
 {
     return Vector(x - rho.x, y - rho.y);
 }
 
 inline Vector Vector::operator*(float scalar) const
 {
     return Vector(x * scalar, y * scalar);
 }
 
 inline Vector Vector::operator/(float scalar) const
 {
     return Vector(x / scalar, y / scalar);
 }
 
 inline float Vector::operator*(const Vector &rho) const
 {
     return x * rho.x + y * rho.y;
 }
 
 inline bool Vector::operator==(const Vector &rho) const
 {
     return x == rho.x && y == rho.y;
 }
 
 inline bool Vector::operator!=(const Vector &rho) const
 {
     return x != rho.x || y != rho.y;
 }
 
 inline Vector& Vector::operator+=(const Vector &other)
 {
     x += other.x;
     y += other.y;
     return *this;
 }
 
 inline Vector& Vector::operator*=(float scalar)
 {
     x *= scalar;
     y *= scalar;
     return *this;
 }
 
 inline Vector Vector::perpendicular() const
 {
     return Vector(y, -x);
 }
 
 inline Vector Vector::normalized() const
 {
     Vector v = *this;
     const float l = length();
     if (l > 0) {
         v.x /= l;
         v.y /= l;
     }
     return v;
 }
 
 inline float Vector::length() const
 {
     return std::sqrt(lengthSquared());
 }
 
 inline float Vector::lengthSquared() const
 {
     return d[0] * d[0] + d[1] * d[1];
 }
 
 inline float Vector::distance(const Vector &rho) const
 {
     return (*this - rho).length();
 }
 
 inline std::ostream& operator<<(std::ostream &stream, const Vector v)
 {
     stream <<"Vector("<<v.x<<", "<<v.y<<")";
     return stream;
 }
 
 #endif // VECTOR_H
 