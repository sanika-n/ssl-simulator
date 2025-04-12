/***************************************************************************
 *   Copyright 2015 Michael Eischer, Philipp Nordhus                       *
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

 #ifndef TIMER_H
 #define TIMER_H
 
 #include <QtGlobal>
 #include <QObject>
 
 /*!
  * \class Timer
  * \brief A time-tracking class with optional speed scaling.
  *
  * This class provides a high-resolution timer that can be scaled (sped up or slowed down),
  * reset, or set manually. It can be used to simulate different time speeds.
  *
  * \note The class tracks time in microseconds (qint64).
  */
 class Timer : public QObject
 {
     Q_OBJECT
 
 public:
     /*!
      * \brief Constructs a Timer with default scaling of 1.0 and resets time.
      */
     Timer();
 
     /*!
      * \brief Returns the current scaling factor.
      * \return Scaling value (1.0 = real-time, 2.0 = 2× faster, etc.)
      */
     double scaling() const { return m_scaling; }
 
     /*!
      * \brief Sets the scaling factor for time progression.
      * \param scaling The new scaling value.
      */
     void setScaling(double scaling);
 
     /*!
      * \brief Resets the timer to 0 while keeping the current scaling factor.
      */
     void reset();
 
     /*!
      * \brief Returns the current scaled time since reset.
      * \return Current time in microseconds.
      */
     qint64 currentTime() const;
 
     /*!
      * \brief Manually sets the timer to a specific time and scaling.
      * \param time New time value to set (in microseconds).
      * \param scaling New scaling factor.
      */
     void setTime(qint64 time, double scaling);
 
 signals:
     /*!
      * \brief Emitted when the scaling factor is changed.
      * \param scaling The new scaling value.
      */
     void scalingChanged(float scaling);
 
 public:
     /*!
      * \brief Returns the current system time in microseconds.
      * \return System time in microseconds since epoch or reference point.
      */
     static qint64 systemTime();
 
 private:
     double m_scaling; //!< Time scaling factor (1.0 = real-time)
     qint64 m_start;   //!< System time when the timer was last reset or scaled
     qint64 m_offset;  //!< Offset to simulate elapsed time since start
 };
 
 #endif // TIMER_H
 