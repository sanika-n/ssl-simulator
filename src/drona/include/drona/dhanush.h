/*
 * The simulator is divided into various components.
 * If you are versed in mythology, you may be able to
 * guess each component's purpose.
 *
 * Dhanush: This component "fires" the velocity data to the simulator,
 * informing it of the bots’ updated motion commands.
 */

#ifndef DHANUSH_H
#define DHANUSH_H

#include "protobuf/ssl_simulation_robot_control.pb.h"
#include <QObject>
#include <QUdpSocket>
#include <QString>
#include <QNetworkDatagram>
#include <google/protobuf/repeated_field.h>

/**
 * @brief Represents a single robot's motion command and identity.
 *
 * This class contains data like linear/angular velocities, kick speed,
 * team color, and ID. It inherits from QObject so it can be used with
 * Qt’s signal-slot mechanism if needed in the future.
 */
class BotPacket : public QObject
{
    Q_OBJECT

public:
    ///default constructor. Initializes all values to zero.
    BotPacket()
        : vel_x(0.0f), vel_y(0.0f), vel_angular(0.0f),
          kick_speed(0.0f), id(0), is_blue(false) {}

    ///parameterized constructor to initialize with ID and team color.
    BotPacket(int id, bool is_blue)
        : vel_x(0.0f), vel_y(0.0f), vel_angular(0.0f),
          kick_speed(0.0f), id(id), is_blue(is_blue) {}

    ///default destructor. Safe due to QObject parenting.
    ~BotPacket() = default;

    //motion control data
    float vel_x;       ///< Velocity in the x-direction (forward)
    float vel_y;       ///< Velocity in the y-direction (left)
    float vel_angular; ///< Angular velocity (rotation)

    //bot identity and action
    int id;            ///< Unique identifier for the robot
    bool is_blue;      ///< True if robot is on the blue team
    float kick_speed;  ///< Kick strength
};


/**
 * @brief Sends velocity commands to bots via UDP using protobuf.
 *
 * Dhanush is responsible for packaging motion data into
 * protobuf messages and sending them to the SSL simulator.
 * Inherits from QObject to use Qt's memory management and signals/slots.
 */
class Dhanush : public QObject
{
    Q_OBJECT

public:
    Dhanush();   ///< Constructor- sets up the socket
    ~Dhanush();  ///< Destructor -  cleans up the socket

public slots:
    /**
     * @brief Sends velocity commands using protobuf and UDP.
     * @param packet Pointer to an array of BotPacket objects.
     */
    void send_velocity(BotPacket* packet);

private:
    /**
     * @brief Temporary structure for representing bot metadata.
     *        (Not currently used outside declaration.)
     */
    struct Bot {
        int x, y;
        int id;
        bool is_blue;

        Bot(int x, int y, int id, bool is_blue = false)
            : x(x), y(y), id(id), is_blue(is_blue) {}
    };

    QUdpSocket* socket;                     ///< UDP socket for sending commands
    sslsim::RobotCommand* command = nullptr;///< Pointer to robot command (protobuf)
    QNetworkDatagram datagram;              ///< Not used yet – placeholder
    QByteArray buffer;                      ///< Used for serialized message data

    void moveToPosition(float x, float y);  ///< [Unimplemented] Move to a specified position
};

#endif // DHANUSH_H
