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

#ifndef SIMFIELD_H
#define SIMFIELD_H
/**
 * @file simfield.h
 * @brief Simulates the field
 * 
 * The file defines the simfield class under simulator
 */
#include "protobuf/world.pb.h"
#include <QList>
#include <btBulletDynamicsCommon.h>
#include <optional>

/**
 * Declaring the namespaces under which simfield is a class
 */
namespace camun {
    namespace simulator {
        class SimField;
    }
}
/**
 * @class SimField
 * @brief Incudes the funcions to add objects
 *  
 */
class camun::simulator::SimField
{
public:
    /**
     * @brief Initializes the constructor with its parameters
     * @param *world Refers to the value of the pointer world
     * @param &geometry Refers to the adress of the geometry parameter
     */
    SimField(btDiscreteDynamicsWorld *world, const world::Geometry &geometry);
    ~SimField();
    SimField(const SimField&) = delete;
    SimField& operator=(const SimField&) = delete;

private:
    /**
     * @brief Adds objects to the field
     * @param *shape Refers to the value of the shape pointer
     * @param &transform Refers to the adress of the transform of the object
     * @param restitution The value of the damping provide when collision happens 
     * @param friction The value of the friction coefficient
     */
    void addObject(btCollisionShape *shape, const btTransform &transform, float restitution, float friction);

private:
    btDiscreteDynamicsWorld *m_world;
    btCollisionShape *m_plane;
    btCollisionShape *m_goalSide;
    btCollisionShape *m_goalBack;
    std::optional<btBoxShape> m_cornerBlockShape;
    std::optional<btBoxShape> m_goalLineBoundaryShape;
    QList<btCollisionObject*> m_objects;
};

#endif // SIMFIELD_H
