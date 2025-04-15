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

 #ifndef RNG_H
 #define RNG_H
 
 #include "vector.h"
 #include <inttypes.h>
 
 /*!
  * \class RNG
  * \brief Lightweight random number generator for vectors, floats, and normal distributions.
  *
  * Provides uniform and normal distribution methods for scalar values and vectors.
  * Internal state is based on 3 seeds.
  */
 class RNG
 {
 public:
     /*!
      * \brief Construct RNG with optional initial seed.
      * \param seed Initial seed value (default is 0).
      */
     explicit RNG(uint32_t seed = 0);
 
     /*!
      * \brief Seed the RNG manually.
      * \param seed The seed value.
      */
     void seed(uint32_t seed);
 
     /*!
      * \brief Generate a uniformly distributed 32-bit integer.
      * \return Random unsigned 32-bit integer.
      */
     uint32_t uniformInt();
 
     /*!
      * \brief Generate a random floating point number in the range [0, 1].
      * \return Random number drawn from uniform distribution.
      */
     double uniform();
 
     /*!
      * \brief Generate a random strictly positive floating point number.
      * \return A value > 0 from uniform distribution.
      */
     double uniformPositive();
 
     /*!
      * \brief Generate a random float in the range [min, max].
      * \param min Lower bound
      * \param max Upper bound
      * \return Random float in range.
      */
     float uniformFloat(float min, float max);
 
     /*!
      * \brief Generate a 2D vector with uniform random components in [0, 1].
      * \return Random vector.
      */
     Vector uniformVector();
 
     /*!
      * \brief Generate a random vector with each component in custom ranges.
      * \param min Minimum bounds for each component.
      * \param max Maximum bounds for each component.
      * \return Vector with random components between given bounds.
      */
     Vector uniformVectorIn(Vector min, Vector max);
 
     /*!
      * \brief Generate a random number from a normal distribution.
      * \param sigma Standard deviation
      * \param mean Mean value (default 0.0)
      * \return Normally distributed number.
      */
     double normal(double sigma, double mean = 0.0);
 
     /*!
      * \brief Generate a 2D vector with components from a normal distribution.
      * \param sigma Standard deviation
      * \param mean Mean value (default 0.0)
      * \return Normally distributed vector.
      */
     Vector normalVector(double sigma, double mean = 0.0);
 
 private:
     uint32_t m_s1; //!< Internal seed state 1
     uint32_t m_s2; //!< Internal seed state 2
     uint32_t m_s3; //!< Internal seed state 3
 };
 
 /*!
  * \brief Generate a random floating point number in the range [0, 1]
  * \return A random number drawn from a uniform distribution [0, 1]
  */
 inline double RNG::uniform()
 {
     return uniformInt() / 4294967296.0;
 }
 
 /*!
  * \brief Generate a random floating point number in the range [min, max]
  * \return A random number drawn from a uniform distribution [min, max]
  */
 inline float RNG::uniformFloat(float min, float max)
 {
     return min + (uniformInt() / 4294967296.0f) * (max - min);
 }
 
 /*!
  * \brief Initialize a vector with two random values drawn from a uniform distribution
  *
  * \sa uniform
  * \return A random vector drawn from a uniform distribution
  */
 inline Vector RNG::uniformVector()
 {
     float x = uniform();
     return Vector(x, uniform());
 }
 
 /*!
  * \brief Initialize a vector with two random values drawn from a uniform distribution (x from [min.x, max.x], y from [min.y, max.y])
  *
  * \sa uniformVector
  * \return A random vector drawn from a uniform distribution (x from [min.x, max.x], y from [min.y, max.y])
  */
 inline Vector RNG::uniformVectorIn(Vector min, Vector max)
 {
     float x = uniformFloat(min.x, max.x);
     return Vector(x, uniformFloat(min.y, max.y));
 }
 
 /*!
  * \brief Generate a random value drawn from a normal distribution
  * \param sigma Standard deviation
  * \param mean Expected value
  * \return A random value drawn from a normal distribution
  */
 inline double RNG::normal(double sigma, double mean)
 {
     return normalVector(sigma, mean).x;
 }
 
 #endif // RNG_H
 