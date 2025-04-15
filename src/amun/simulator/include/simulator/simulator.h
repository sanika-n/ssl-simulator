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

 #ifndef SIMULATOR_H
 #define SIMULATOR_H
 
 /**
 * @file simulator.h
 * @brief Main simulator controller for the SSL simulation environment.
 */
 
 #include "protobuf/command.h"
 #include "protobuf/status.h"
 #include "protobuf/sslsim.h"
 #include <QList>
 #include <QMap>
 #include <QPair>
 #include <QQueue>
 #include <QByteArray>
 #include <tuple>
 #include <random>
 
 /**
 * @def SIMULATOR_SCALE
 * @brief Scaling factor for the physics simulation
 * Higher values break the rolling friction of the ball
 */
 const float SIMULATOR_SCALE = 10.0f;
 
 /**
 * @def SUB_TIMESTEP
 * @brief Time step size for physics simulation (in seconds)
 * Defines the granularity of the physics simulation (200 Hz)
 */
 const float SUB_TIMESTEP = 1/200.f;
 
 /**
 * @def COLLISION_MARGIN
 * @brief Safety margin used for collision detection (in meters)
 */
 const float COLLISION_MARGIN = 0.04f;
 
 /**
 * @def FOCAL_LENGTH
 * @brief Focal length for the simulated cameras (in pixels)
 */
 const unsigned FOCAL_LENGTH = 390;
 
 class QByteArray;
 class QTimer;
 class Timer;
 class SSL_GeometryFieldSize;
 
 namespace camun {
     namespace simulator {
         class SimRobot;
         class Simulator;
         class ErrorAggregator;
         struct SimulatorData;
 
         /**
         * @enum ErrorSource
         * @brief Identifies the source of simulation errors
         */
         enum class ErrorSource {
             BLUE,    /**< Error from blue team */
             YELLOW,  /**< Error from yellow team */
             CONFIG   /**< Error in simulator configuration */
         };
     }
 }
 
 /**
 * @class camun::simulator::Simulator
 * @brief Main controller for the SSL physics simulation
 * The Simulator class manages the physics world, robots, ball, and vision systems.
 * It handles command processing, physics updates, and vision packet generation.
 */
 class camun::simulator::Simulator : public QObject
 {
     Q_OBJECT
 
 public:
     /**
     * @typedef RobotMap
     * @brief Map of robot IDs to robot instances and generation counters
     * First int: Robot ID, Second int: Generation
     */
     typedef QMap<unsigned int, QPair<SimRobot*, unsigned int>> RobotMap;
 
     /**
     * @fn Simulator::Simulator(const Timer *timer, const amun::SimulatorSetup &setup, bool useManualTrigger = false)
     * @brief Constructs the simulator with the specified configuration
     * @param timer Timer used for simulation timing
     * @param setup Initial simulator configuration
     * @param useManualTrigger Whether to use manual triggering instead of automatic updates
     */
     explicit Simulator(const Timer *timer, const amun::SimulatorSetup &setup, bool useManualTrigger = false);
     
     /**
     * @fn Simulator::~Simulator()
     * @brief Destroys the simulator and cleans up all resources
     */
     ~Simulator() override;
     Simulator(const Simulator&) = delete;
     Simulator& operator=(const Simulator&) = delete;
     
     /**
     * @fn void Simulator::handleSimulatorTick(double timeStep)
     * @brief Processes a single simulation tick
     * Updates the physics world and all objects within it for the specified time step.
     * @param timeStep Time in seconds to simulate
     */
     void handleSimulatorTick(double timeStep);
     
     /**
     * @fn void Simulator::seedPRGN(uint32_t seed)
     * @brief Seeds the pseudo-random number generator
     * Used to make simulation results reproducible with the same seed.
     * @param seed Seed value for the random number generator
     */
     void seedPRGN(uint32_t seed);
 
 signals:
     /**
     * @fn void Simulator::gotPacket(const QByteArray &data, qint64 time, QString sender)
     * @brief Signal emitted when a vision packet is generated
     * @param data Packet data
     * @param time Timestamp of the packet
     * @param sender Identifier of the sender
     */
     void gotPacket(const QByteArray &data, qint64 time, QString sender);
     
     /**
     * @fn void Simulator::sendStatus(const Status &status)
     * @brief Signal emitted when a status update is available
     * @param status Current simulator status
     */
     void sendStatus(const Status &status);
     
     /**
     * @fn void Simulator::sendRadioResponses(const QList<robot::RadioResponse> &responses)
     * @brief Signal emitted when robot radio responses are available
     * @param responses List of radio responses from robots
     */
     void sendRadioResponses(const QList<robot::RadioResponse> &responses);
     
     /**
     * @fn void Simulator::sendRealData(const QByteArray& data)
     * @brief Signal emitted with raw simulator state data
     * @param data Serialized amun::SimulatorState data
     */
     void sendRealData(const QByteArray& data);
     
     /**
     * @fn void Simulator::sendSSLSimError(const QList<SSLSimError>& errors, ErrorSource source)
     * @brief Signal emitted when simulation errors occur
     * @param errors List of simulation errors
     * @param source Source of the errors (blue team, yellow team, or configuration)
     */
     void sendSSLSimError(const QList<SSLSimError>& errors, ErrorSource source);
 
 public slots:
     /**
     * @fn void Simulator::handleCommand(const Command &command)
     * @brief Processes a simulator command
     * @param command Command to process
     */
     void handleCommand(const Command &command);
     
     /**
     * @fn void Simulator::handleRadioCommands(const SSLSimRobotControl& control, bool isBlue, qint64 processingStart)
     * @brief Processes robot control commands
     * @param control Robot control commands
     * @param isBlue Whether the commands are for the blue team
     * @param processingStart Time when command processing started
     */
     void handleRadioCommands(const SSLSimRobotControl& control, bool isBlue, qint64 processingStart);
     
     /**
     * @fn void Simulator::setScaling(double scaling)
     * @brief Sets the time scaling factor for simulation
     * @param scaling Time scaling factor (1.0 = real-time)
     */
     void setScaling(double scaling);
     
     /**
     * @fn void Simulator::setFlipped(bool flipped)
     * @brief Sets whether the field orientation is flipped
     * @param flipped true if the field is flipped, false otherwise
     */
     void setFlipped(bool flipped);
     
     /**
     * @fn void Simulator::safelyTeleportBall(const float x, const float y)
     * @brief Teleports the ball while avoiding robot collisions
     * Checks for possible collisions with robots at the target position
     * and moves robots out of the way using teleportRobotToFreePosition.
     * @param x Target x-coordinate for the ball
     * @param y Target y-coordinate for the ball
     */
     void safelyTeleportBall(const float x, const float y);
     
     /**
     * @fn void Simulator::process()
     * @brief Processes pending simulation updates
     * Called periodically to update the simulation state
     */
     void process();
 
 private slots:
     /**
     * @fn void Simulator::sendVisionPacket()
     * @brief Sends simulated vision packets
     * Creates and emits SSL vision detection packets
     */
     void sendVisionPacket();
 
 private:
     /**
     * @fn void Simulator::sendSSLSimErrorInternal(ErrorSource source)
     * @brief Internal method to send simulation errors
     * @param source Source of the errors
     */
     void sendSSLSimErrorInternal(ErrorSource source);
     
     /**
     * @fn void Simulator::resetFlipped(RobotMap &robots, float side)
     * @brief Resets robot positions when the field is flipped
     * @param robots Map of robots to reset
     * @param side Side of the field the robots are on
     */
     void resetFlipped(RobotMap &robots, float side);
     
     /**
     * @fn std::tuple<QList<QByteArray>, QByteArray, qint64> Simulator::createVisionPacket()
     * @brief Creates vision detection and geometry packets
     * @return Tuple containing (detection packets, geometry packet, timestamp)
     */
     std::tuple<QList<QByteArray>, QByteArray, qint64> createVisionPacket();
     
     /**
     * @fn void Simulator::resetVisionPackets()
     * @brief Resets all pending vision packets
     * Used when simulation parameters change
     */
     void resetVisionPackets();
     
     /**
     * @fn void Simulator::setTeam(RobotMap &list, float side, const robot::Team &team, QMap<uint32_t, robot::Specs>& specs)
     * @brief Sets up a team of robots
     * @param list Map to store the robots
     * @param side Side of the field the team is on
     * @param team Team configuration
     * @param specs Map of robot specifications
     */
     void setTeam(RobotMap &list, float side, const robot::Team &team, QMap<uint32_t, robot::Specs>& specs);
     
     /**
     * @fn void Simulator::moveBall(const sslsim::TeleportBall &ball)
     * @brief Teleports the ball to a specified position
     * @param ball Teleport command with target position and velocity
     */
     void moveBall(const sslsim::TeleportBall &ball);
     
     /**
     * @fn void Simulator::moveRobot(const sslsim::TeleportRobot &robot)
     * @brief Teleports a robot to a specified position
     * @param robot Teleport command with target position and orientation
     */
     void moveRobot(const sslsim::TeleportRobot &robot);
     
     /**
     * @fn void Simulator::teleportRobotToFreePosition(SimRobot *robot)
     * @brief Teleports a robot to a position free of collisions
     * @param robot Robot to teleport
     */
     void teleportRobotToFreePosition(SimRobot *robot);
     
     /**
     * @fn void Simulator::initializeDetection(SSL_DetectionFrame *detection, std::size_t cameraId)
     * @brief Initializes a detection frame with camera information
     * @param detection Detection frame to initialize
     * @param cameraId ID of the camera for this frame
     */
     void initializeDetection(SSL_DetectionFrame *detection, std::size_t cameraId);
 
 private:
     /// @brief Type definition for radio command queue entries
     typedef std::tuple<SSLSimRobotControl, qint64, bool> RadioCommand;
     
     /// @brief Internal simulator data containing physics world and objects
     SimulatorData *m_data;
     
     /// @brief Queue of pending radio commands
     QQueue<RadioCommand> m_radioCommands;
     
     /// @brief Queue of pending vision packets
     QQueue<std::tuple<QList<QByteArray>, QByteArray, qint64>> m_visionPackets;
     
     /// @brief Timers for vision packet delivery
     QQueue<QTimer *> m_visionTimers;
     
     /// @brief Whether the simulator is running in partial mode
     bool m_isPartial;
     
     /// @brief Timer for simulation timing
     const Timer *m_timer;
     
     /// @brief Timer for manual simulation triggering
     QTimer *m_trigger;
     
     /// @brief Current simulation time
     qint64 m_time;
     
     /// @brief Time when the last status was sent
     qint64 m_lastSentStatusTime;
     
     /// @brief Time scaling factor for simulation
     double m_timeScaling;
     
     /// @brief Whether the simulator is enabled
     bool m_enabled;
     
     /// @brief Whether robot kickers are allowed to charge
     bool m_charge;
     
     /// @brief Total vision delay (systemDelay + visionProcessingTime)
     qint64 m_visionDelay;
     
     /// @brief Vision processing time component of vision delay
     qint64 m_visionProcessingTime;
 
     /// @brief Minimum time between robot detections
     qint64 m_minRobotDetectionTime = 0;
     
     /// @brief Minimum time between ball detections
     qint64 m_minBallDetectionTime = 0;
     
     /// @brief Time when the ball was last sent in a vision packet
     qint64 m_lastBallSendTime = 0;
     
     /// @brief Map of frame numbers by camera ID
     std::map<qint64, unsigned> m_lastFrameNumber;
     
     /// @brief Error aggregator for collecting and reporting errors
     ErrorAggregator *m_aggregator;
 
     /// @brief Random number generator for various simulation randomizations
     std::mt19937 rand_shuffle_src = std::mt19937(std::random_device()());
 };
 
 #endif // SIMULATOR_H