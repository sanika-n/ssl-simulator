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

 #ifndef SIMBALL_H
 #define SIMBALL_H
 
 /**
 * @file simball.h
 * @brief Simulates a ball using the Bullet physics engine.
 */
 
 #include "protobuf/command.pb.h"
 #include "protobuf/sslsim.h"
 #include <btBulletDynamicsCommon.h>
 #include "simfield.h"
 #include <QObject>
 
 /**
 * @def BALL_RADIUS
 * @brief The radius of the simulated ball in meters
 */
 static const float BALL_RADIUS = 0.0215f;
 
 /**
 * @def BALL_MASS
 * @brief The mass of the simulated ball in kilograms
 */
 static const float BALL_MASS = 0.046f;
 
 /**
 * @def BALL_DECELERATION
 * @brief The deceleration factor applied to the ball to simulate rolling friction
 */
 static const float BALL_DECELERATION = 0.5f;
 
 class RNG;
 class SSL_DetectionBall;
 
 namespace camun {
     namespace simulator {
         class SimBall;
         enum class ErrorSource;
     }
 }
 
 /**
 * @class camun::simulator::SimBall
 * @brief Physics-based simulation of an SSL ball
 * The SimBall class creates and manages a physical representation of a ball
 * in a Bullet physics simulation. It handles ball movement, interactions with
 * the field and robots, and vision detection simulation.
 */
 class camun::simulator::SimBall: public QObject
 {
     Q_OBJECT
 public:
     /**
     * @fn SimBall::SimBall(RNG *rng, btDiscreteDynamicsWorld *world)
     * @brief Constructs a simulated ball
     * @param rng Random number generator for noise simulation
     * @param world Bullet physics world in which the ball exists
     */
     SimBall(RNG *rng, btDiscreteDynamicsWorld *world);
 
     /**
     * @fn SimBall::~SimBall()
     * @brief Destroys the simulated ball and cleans up all physics resources
     */
     ~SimBall();
     SimBall(const SimBall&) = delete;
     SimBall& operator=(const SimBall&) = delete;
 
 signals:
     /**
     * @fn void SimBall::sendSSLSimError(const SSLSimError& error, ErrorSource s)
     * @brief Signal emitted when a simulation error occurs
     * @param error Error message to be sent
     * @param s Source of the error
     */
     void sendSSLSimError(const SSLSimError& error, ErrorSource s);
 
 public:
     /**
     * @fn void SimBall::begin()
     * @brief Prepares the ball for a simulation step
     * This is called at the beginning of each simulation step to prepare the ball
     * for updates and interactions.
     */
     void begin();
 
     /**
     * @fn bool SimBall::update(SSL_DetectionBall *ball, float stddev, float stddevArea, const btVector3 &cameraPosition, bool enableInvisibleBall, float visibilityThreshold, btVector3 positionOffset)
     * @brief Updates a vision detection packet with ball information
     * Fills the SSL vision detection packet with the ball's position, adding simulated vision noise.
     * @param ball SSL detection ball message to be filled
     * @param stddev Standard deviation for position noise
     * @param stddevArea Standard deviation for area calculation
     * @param cameraPosition Position of the camera in the physics world
     * @param enableInvisibleBall Whether to allow the ball to be invisible in some situations
     * @param visibilityThreshold Threshold for determining ball visibility
     * @param positionOffset Offset to apply to the ball's position
     * @return true if the ball was successfully updated, false otherwise
     */
     bool update(SSL_DetectionBall *ball, float stddev, float stddevArea, const btVector3 &cameraPosition,
                bool enableInvisibleBall, float visibilityThreshold, btVector3 positionOffset);
 
     /**
     * @fn void SimBall::move(const sslsim::TeleportBall &ball)
     * @brief Sets a teleport command for manually positioning the ball
     * @param ball Teleport command with target position and velocity
     */
     void move(const sslsim::TeleportBall &ball);
 
     /**
     * @fn void SimBall::kick(const btVector3 &power)
     * @brief Applies a kick force to the ball
     * @param power Vector representing the direction and magnitude of the kick
     */
     void kick(const btVector3 &power);
 
     /**
     * @fn btVector3 SimBall::position() const
     * @brief Gets the ball's current position projected onto the field
     * @return 2D position of the ball on the field (z component is not included)
     */
     btVector3 position() const;
 
     /**
     * @fn btVector3 SimBall::speed() const
     * @brief Gets the ball's current velocity
     * @return Velocity vector of the ball in the physics world
     */
     btVector3 speed() const;
 
     /**
     * @fn void SimBall::writeBallState(world::SimBall *ball) const
     * @brief Updates a world state ball with simulation data
     * @param ball World state ball message to be filled
     */
     void writeBallState(world::SimBall *ball) const;
     
     /**
     * @fn void SimBall::restoreState(const world::SimBall &ball)
     * @brief Restores the ball's state from a world state message
     * @param ball World state ball message to restore from
     */
     void restoreState(const world::SimBall &ball);
 
     /**
     * @fn btRigidBody *SimBall::body() const
     * @brief Gets the ball's Bullet rigid body
     * @return Pointer to the ball's physics body
     */
     btRigidBody *body() const { return m_body; }
 
     /**
     * @fn bool SimBall::isInvalid() const
     * @brief Checks if the ball is in an invalid state
     * @return true if the ball is invalid (e.g., off-field), false otherwise
     */
     bool isInvalid() const;
 
     /**
     * @fn bool SimBall::addDetection(SSL_DetectionBall *ball, btVector3 pos, float stddev, float stddevArea, const btVector3 &cameraPosition, bool enableInvisibleBall, float visibilityThreshold, btVector3 positionOffset)
     * @brief Adds a simulated ball detection at the specified position
     * This can be used to add ball mis-detections to the simulation for testing vision filtering.
     * @param ball SSL detection ball message to be filled
     * @param pos Position to use for the detection
     * @param stddev Standard deviation for position noise
     * @param stddevArea Standard deviation for area calculation
     * @param cameraPosition Position of the camera in the physics world
     * @param enableInvisibleBall Whether to allow the ball to be invisible in some situations
     * @param visibilityThreshold Threshold for determining ball visibility
     * @param positionOffset Offset to apply to the ball's position
     * @return true if the detection was successfully added, false otherwise
     */
     bool addDetection(SSL_DetectionBall *ball, btVector3 pos, float stddev, float stddevArea, const btVector3 &cameraPosition,
                       bool enableInvisibleBall, float visibilityThreshold, btVector3 positionOffset);
 
 private:
     /// @brief Random number generator for noise simulation
     RNG *m_rng;
     
     /// @brief Bullet physics world in which the ball exists
     btDiscreteDynamicsWorld *m_world;
     
     /// @brief Collision shape for the ball (sphere)
     btCollisionShape *m_sphere;
     
     /// @brief Main rigid body for the ball
     btRigidBody *m_body;
     
     /// @brief Motion state of the ball
     btMotionState *m_motionState;
     
     /// @brief Current teleport command
     sslsim::TeleportBall m_move;
 };
 
 #endif // SIMBALL_H