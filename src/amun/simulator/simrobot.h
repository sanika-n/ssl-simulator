/***************************************************************************
 *   Copyright 2015 Michael Eischer, Jan Kallwies, Philipp Nordhus         *
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

#ifndef SIMROBOT_H
#define SIMROBOT_H

/**
* @file simrobot.h
* @brief Simulates a robot using the Bullet physics engine.
*/

#include "protobuf/command.pb.h"
#include "protobuf/robot.pb.h"
#include "protobuf/sslsim.h"
#include <QList>
#include <Eigen/Dense>
#include <Eigen/QR>
#include <btBulletDynamicsCommon.h>

class RNG;
class SSL_DetectionRobot;

namespace camun {
    namespace simulator {
        class SimBall;
        class SimRobot;
        enum class ErrorSource;
    }
}

/**
* @class camun::simulator::SimRobot
* @brief Physics-based simulation of an SSL robot
* The SimRobot class creates and manages a physical representation of a robot
* in a Bullet physics simulation. It handles robot movement, ball interaction,
* command processing, and state reporting.
*/
class camun::simulator::SimRobot: public QObject
{
    Q_OBJECT
public:
    /**
    * @fn SimRobot::SimRobot(RNG *rng, const robot::Specs &specs, btDiscreteDynamicsWorld *world, const btVector3 &pos, float dir)
    * @brief Constructs a simulated robot
    * @param rng Random number generator for noise simulation
    * @param specs Robot specifications (dimensions, capabilities, etc.)
    * @param world Bullet physics world in which the robot exists
    * @param pos Initial position of the robot
    * @param dir Initial orientation of the robot (radians)
    */
    SimRobot(RNG *rng, const robot::Specs &specs, btDiscreteDynamicsWorld *world, const btVector3 &pos, float dir);

    /**
    * @fn SimRobot::~SimRobot()
    * @brief Destroys the simulated robot and cleans up all physics resources
    */
    ~SimRobot();

    SimRobot(const SimRobot&) = delete;
    SimRobot& operator=(const SimRobot&) = delete;

signals:
    /**

    * @fn void SimRobot::sendSSLSimError(const SSLSimError& error, ErrorSource s)
    * @brief Signal emitted when a simulation error occurs
    * @param error Error message to be sent
    * @param s Source of the error
    */
    void sendSSLSimError(const SSLSimError& error, ErrorSource s);

public:
    /**
    * @fn void SimRobot::begin(SimBall *ball, double time)
    * @brief Updates the robot state for a simulation step
    * This is the main update function called each simulation step. It processes
    * commands, applies forces to the robot, handles ball interaction, and manages
    * dribbling and kicking operations.
    * @param ball Pointer to the simulation ball
    * @param time Time delta for this simulation step (seconds)
    */
    void begin(SimBall *ball, double time);

    /**
    * @fn bool SimRobot::canKickBall(SimBall *ball) const
    * @brief Checks if the robot can kick the ball
    * Determines if the ball is in contact with the dribbler or held by the
    * constraint, making it eligible for kicking.

    * @param ball Pointer to the simulation ball
    * @return true if the ball can be kicked, false otherwise
    */
    bool canKickBall(SimBall *ball) const;

    /**
    * @fn void SimRobot::tryKick(SimBall *ball, float power, double time)
    * @brief Attempts to kick the ball with the specified power
    * @param ball Pointer to the simulation ball
    * @param power Kick power
    * @param time Time delta for this simulation step (seconds)
    */
    void tryKick(SimBall *ball, float power, double time);

    /**
    * @fn robot::RadioResponse SimRobot::setCommand(const sslsim::RobotCommand &command, SimBall *ball, bool charge, float rxLoss, float txLoss)
    * @brief Sets the robot command and returns radio feedback
    * Processes a new command for the robot and generates radio feedback that would
    * be sent by a real robot.
    * @param command Command to be executed by the robot
    * @param ball Pointer to the simulation ball
    * @param charge Whether the robot's kicker is allowed to charge
    * @param rxLoss Simulated radio packet loss for receiving
    * @param txLoss Simulated radio packet loss for transmitting
    * @return Radio response from the robot
    */
    robot::RadioResponse setCommand(const sslsim::RobotCommand &command, SimBall *ball, bool charge, float rxLoss, float txLoss);

    /**
    * @fn void SimRobot::update(SSL_DetectionRobot *robot, float stddev_p, float stddev_phi, qint64 time, btVector3 positionOffset)
    * @brief Updates a vision detection packet with robot information
    * Fills the SSL vision detection packet with the robot's position and orientation,
    * adding simulated vision noise.
    * @param robot SSL detection robot message to be filled
    * @param stddev_p Standard deviation for position noise
    * @param stddev_phi Standard deviation for orientation noise
    * @param time Current simulation time
    * @param positionOffset Offset to apply to the robot's position
    */
    void update(SSL_DetectionRobot *robot, float stddev_p, float stddev_phi, qint64 time, btVector3 positionOffset);

    /**
    * @fn void SimRobot::update(world::SimRobot *robot, SimBall *ball) const
    * @brief Updates a world state robot with simulation data
    * @param robot World state robot message to be filled
    * @param ball Pointer to the simulation ball
    */
    void update(world::SimRobot *robot, SimBall *ball) const;

    /**
    * @fn void SimRobot::restoreState(const world::SimRobot &robot)
    * @brief Restores the robot's state from a world state message
    * @param robot World state robot message to restore from
    */
    void restoreState(const world::SimRobot &robot);

    /**
    * @fn void SimRobot::move(const sslsim::TeleportRobot &robot)
    * @brief Sets a teleport command for manually positioning the robot
    * @param robot Teleport command with target position and orientation
    */
    void move(const sslsim::TeleportRobot &robot);

    /**
    * @fn bool SimRobot::isFlipped()
    * @brief Checks if the robot has flipped over
    * @return true if the robot is upside down, false otherwise
    */
    bool isFlipped();

    /**
    * @fn btVector3 SimRobot::position() const
    * @brief Gets the robot's current position
    * @return Position of the robot in the physics world
    */
    btVector3 position() const;

    /**
    * @fn btVector3 SimRobot::dribblerCorner(bool left) const
    * @brief Gets the position of one corner of the dribbler
    * @param left true for the left corner, false for the right corner
    * @return Position of the specified corner in the physics world
    */
    btVector3 dribblerCorner(bool left) const;

    /**
    * @fn qint64 SimRobot::getLastSendTime() const
    * @brief Gets the timestamp of the last vision update
    * @return Time of the last vision update
    */
    qint64 getLastSendTime() const { return m_lastSendTime; }

    /**
    * @fn void SimRobot::setDribbleMode(bool perfectDribbler)
    * @brief Sets the dribbler mode (normal or perfect)
    * @param perfectDribbler true for perfect dribbler mode (using constraints), false for physics-based dribbling
    */
    void setDribbleMode(bool perfectDribbler);

    /**
    * @fn void SimRobot::stopDribbling()
    * @brief Stops the dribbler mechanism
    */
    void stopDribbling();

    /**
    * @fn const robot::Specs& SimRobot::specs() const
    * @brief Gets the robot specifications
    * @return Reference to the robot's specifications
    */
    const robot::Specs& specs() const { return m_specs; }

private:
    /**
    * @fn btVector3 SimRobot::relativeBallSpeed(SimBall *ball) const
    * @brief Calculates the ball's speed relative to the robot
    * @param ball Pointer to the simulation ball
    * @return Velocity of the ball in the robot's local coordinate system
    */
    btVector3 relativeBallSpeed(SimBall *ball) const;

    /**
    * @fn float SimRobot::bound(float acceleration, float oldSpeed, float speedupLimit, float brakeLimit) const
    * @brief Bounds acceleration based on direction and current speed
    * Uses different limits for accelerating and braking to simulate realistic robot behavior.
    * @param acceleration Desired acceleration
    * @param oldSpeed Current speed
    * @param speedupLimit Maximum acceleration when speeding up
    * @param brakeLimit Maximum deceleration when braking
    * @return Bounded acceleration value
    */
    float bound(float acceleration, float oldSpeed, float speedupLimit, float brakeLimit) const;

    /**
    * @fn void SimRobot::calculateDribblerMove(const btVector3 pos, const btQuaternion rot, const btVector3 linVel, float omega)
    * @brief Updates the dribbler position and velocity based on robot movement
    * @param pos Robot position
    * @param rot Robot orientation
    * @param linVel Robot linear velocity
    * @param omega Robot angular velocity
    */
    void calculateDribblerMove(const btVector3 pos, const btQuaternion rot, const btVector3 linVel, float omega);
    // returns {a_s, a_f, a_phi} bounded

    /**
    * @fn Eigen::Vector3f SimRobot::limitAcceleration(float a_f, float a_s, float a_phi, float v_f, float v_s, float omega) const
    * @brief Limits robot acceleration based on wheel capabilities
    * Uses the velocity coupling matrix to transform robot accelerations to wheel accelerations,
    * applies limits, and transforms back.
    * @param a_f Forward acceleration
    * @param a_s Sideways acceleration
    * @param a_phi Angular acceleration
    * @param v_f Forward velocity
    * @param v_s Sideways velocity
    * @param omega Angular velocity
    * @return Bounded acceleration vector {a_s, a_f, a_phi}
    */
    Eigen::Vector3f limitAcceleration(float a_f, float a_s, float a_phi, float v_f, float v_s, float omega) const;

    /**
    * @fn void SimRobot::dribble(SimBall *ball, float speed)
    * @brief Activates the dribbler with the specified speed
    * @param ball Pointer to the simulation ball
    * @param speed Dribbler speed
    */
    void dribble(SimBall *ball, float speed);

    /**
    * @fn bool SimRobot::handleMoveCommand()
    * @brief Processes teleport commands for manual positioning
    * @return true if a move command was processed, false otherwise
    */
    bool handleMoveCommand();

    /**
    * @fn void SimRobot::reportAccelerationLimits() const
    * @brief Reports the effective acceleration limits to debug output
    */
    void reportAccelerationLimits() const;

    /**
    * @fn void SimRobot::generateVelocityCoupling()
    * @brief Initializes the velocity coupling matrix for wheel transformations
    * Creates the matrix that transforms between robot velocities (forward, sideways, angular)
    * and individual wheel velocities.
    */
    void generateVelocityCoupling();


    /// @brief random number generator for noise simulation
    RNG *m_rng;

    /// @brief Specifications for the robot like capabilities,dimensions,etc.
    robot::Specs m_specs;

    /// @brief Bullet physics world in which the robot exists.
    btDiscreteDynamicsWorld *m_world;

    /// @brief Main rigid body for the robot
    btRigidBody * m_body;

    /// @brief Main dribbler body attached to the main body.
    btRigidBody * m_dribblerBody;

    /// @brief Hinge constraint that connects main dribbler body to the main rigid body.
    btHingeConstraint *m_dribblerConstraint;

    /// @brief collection of collision shapes
    QList<btCollisionShape*> m_shapes;

    /// @brief Motion state of the main robot body.
    btMotionState * m_motionState;

    /// @brief Position of the center of the center of main dribbler body
    btVector3 m_dribblerCenter;

    /// @brief Constraint used in perfect dribbler mode to hold the ball
    std::unique_ptr<btHingeConstraint> m_holdBallConstraint;

    struct Wheel
    {
        float angle;
        btVector3 pos;
        btVector3 dir;
    };


    /// @brief Current teleport command
    sslsim::TeleportRobot m_move;

    /// @brief Current robot command
    sslsim::RobotCommand m_sslCommand;

    /// @brief Whether the kicker is allowed to charge
    bool m_charge;

    /// @brief Whether the kicker is charged and ready to kick
    bool m_isCharged;

    /// @brief Whether the robot is in standby mode
    bool m_inStandby;

    /// @brief  time since the ball was kicked last time
    double m_shootTime;

    /// @brief time since the last command was recieved
    double m_commandTime;

    /// @brief error for sideways velocity (for integral control)
    float error_sum_v_s;
    /// @brief error for forward velocity (for integral control)
    float error_sum_v_f;
    /// @brief error for angular velocity
    float error_sum_omega;

    /// @brief  whether the bot is in perfect dribbler mode
    bool m_perfectDribbler = false;

    qint64 m_lastSendTime = 0;

    Eigen::Matrix<float, 4, 3> m_velocityCoupling;
    Eigen::CompleteOrthogonalDecomposition<Eigen::Matrix<float, 4, 3>> m_inverseCoupling;
};

#endif // SIMROBOT_H
