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

#ifndef MESH_H
#define MESH_H

/**
 * @file mesh.h
 * 
 */

/// Includes (optional)
// #include <QMap>
// #include <QVector>
// #include <QVector3D>
// #include <QString>

/**
 * @class Mesh
 * @brief Creates meshes
 * 
 * This class creates a 3D-Model(meshes) that are used for the bots.
 */

#include <QMap>
#include <QVector>
#include <QVector3D>
#include <QString>

namespace camun {
    namespace simulator {
        class Mesh;
    }
}

class camun::simulator::Mesh
{
public:
    /** 
     * @brief It is the constructor function of the mesh class.
     * @param radius The radius of a robot or a component of it.
     * @param height The height of the robot body.
     * @param angle The angular extent of the mess Section.
     * @param holeSize It is the size of the hole defined in the robots body likely for wheels or for sensors
     * @param boxHeight Height of the bounding box.
     */
    Mesh(float radius, float height, float angle, float holeSize, float boxHeight);

    /**
    * @brief function to create the robot mesh.
    * @param radius The radius of a robot or a component of it.
    * @param height The height of the robot body.
    * @param angle The angular extent of the mess Section.
    */
    void createRobotMeshes(float radius, float height, float angle);

    /**
     * @brief Has a List of all parts or components of the robot body which have in them a collection of points(another list).
     */
    const QList<QList<QVector3D>> &hull() const { return m_hull; }

private:
    /**
     * @brief Adds cover geometry to the robot based on angle range and repetition count (num).
     * @param num it is basically the repetition count or number of points to be added in the region of a particular slice between the start and end angle.
     * @param startAngle As the name suggests it is the start angle from which the mesh is defined.
     * @param endAngle It is the endangle of the mesh definition.
     */
    void addRobotCover(uint num, float startAngle, float endAngle);
    /**
     * @brief Defines the sides of the bot. In the same procedure as above function(addRobotCover)
     * @param right It is the boolean that stores whether it is the left or the right side.
     */
    void addSidePart(uint num, float angleStart, float angleStop, bool right);

private:


    /// @brief A list to store the components which are defined by a list of points.
    QList<QList<QVector3D>> m_hull;
    /// @brief The radius of the robot body
    const float m_radius;
    /// @brief The height of the robot body
    const float m_height;
    /// @brief The angle range of the robot body
    const float m_angle;
    /// @brief The holeSize of the hole in the robot body (Likey a hole for a sensor or a wheel)
    const float m_holeSize;
};

#endif // MESH_H
