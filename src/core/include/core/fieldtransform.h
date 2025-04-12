/***************************************************************************
 *   Copyright 2020 Andreas Wendler                                        *
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

 #ifndef FIELDTRANSFORM_H
 #define FIELDTRANSFORM_H
 
 #include <array>
 #include <QPointF>
 
 /*!
  * \class FieldTransform
  * \brief Applies geometric transformations to positions, velocities, and angles on the field.
  *
  * Supports flipping, affine transformations, and inverse transforms.
  * Used primarily in RoboCup simulations or vision modules to handle side-switching and coordinate transformations.
  */
 class FieldTransform
 {
 public:
     /*!
      * \brief Constructs a FieldTransform with no initial transformation.
      */
     FieldTransform();
 
     /*!
      * \brief Enables or disables horizontal field flipping.
      * \param flip True to enable flipping, false to disable.
      */
     void setFlip(bool flip);
 
     /*!
      * \brief Sets the affine transformation parameters.
      * \param values Array of 6 float values representing the transform matrix.
      */
     void setTransform(const std::array<float, 6> &values);
 
     /*!
      * \brief Applies the transform to the X component of a position.
      * \param x Input X coordinate.
      * \param y Input Y coordinate.
      * \return Transformed X value.
      */
     float applyPosX(float x, float y) const;
 
     /*!
      * \brief Applies the transform to the Y component of a position.
      * \param x Input X coordinate.
      * \param y Input Y coordinate.
      * \return Transformed Y value.
      */
     float applyPosY(float x, float y) const;
 
     /*!
      * \brief Applies the full transform to a QPointF position.
      * \param pos Input position.
      * \return Transformed QPointF.
      */
     QPointF applyPosition(const QPointF &pos) const;
 
     /*!
      * \brief Applies transform to the X component of velocity (or directional speed).
      * \param x Input X component.
      * \param y Input Y component.
      * \return Transformed X velocity.
      */
     float applySpeedX(float x, float y) const;
 
     /*!
      * \brief Applies transform to the Y component of velocity (or directional speed).
      * \param x Input X component.
      * \param y Input Y component.
      * \return Transformed Y velocity.
      */
     float applySpeedY(float x, float y) const;
 
     /*!
      * \brief Applies flipping to an angle.
      * \param angle Original angle in radians or degrees.
      * \return Transformed angle (after flipping, if enabled).
      */
     float applyAngle(float angle) const;
 
     /*!
      * \brief Applies the inverse transform to the X coordinate.
      * \param x Transformed X.
      * \param y Transformed Y.
      * \return Original X before transformation.
      */
     float applyInverseX(float x, float y) const;
 
     /*!
      * \brief Applies the inverse transform to the Y coordinate.
      * \param x Transformed X.
      * \param y Transformed Y.
      * \return Original Y before transformation.
      */
     float applyInverseY(float x, float y) const;
 
     /*!
      * \brief Applies the inverse transform to a position.
      * \param pos Transformed QPointF.
      * \return Original QPointF before transformation.
      */
     QPointF applyInversePosition(const QPointF &pos) const;
 
 private:
     bool m_lastFlipped;             //!< Indicates if the last state was flipped
     bool m_hasTransform;            //!< True if a valid transform has been set
     std::array<float, 6> m_transform; //!< Affine transformation matrix
     float m_flipFactor;             //!< Flip multiplier (+1 or -1)
 };
 
 #endif // FIELDTRANSFORM_H
 