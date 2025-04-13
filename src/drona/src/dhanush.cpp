#include "dhanush.h"  ///< Include the corresponding header file
#include "core/sslprotocols.h"

/// Macro for logging with a consistent tag
#define LOG qDebug() << "[dhanush] : "

// TODO: Replace hardcoded TOTAL_BOTS with dynamic data from the simulator

/// Defines the total number of bots in the simulation
#define TOTAL_BOTS 10

using namespace sslsim;

/// Constructor for the Dhanush class
Dhanush::Dhanush():
    // Create a new QUdpSocket for sending data over UDP
    socket(new QUdpSocket(this))  // 'this' sets the parent so Qt can manage cleanup
{
}

/// Destructor for the Dhanush class
Dhanush::~Dhanush() {
    // Delete the socket manually
    delete socket;
}


/**
 * @brief Prepares and sends velocity control packets to the simulator
 *
 * This method builds a RobotControl protobuf message that contains commands for each robot.
 * Each command specifies velocities in the robot's **local coordinate frame**:
 *  - `forward`: movement along x
 *  - `left`: movement along y
 *  - `angular`: rotation
 *
 * After construction, the message is serialized and sent to the appropriate simulator port
 * using a QUdpSocket.
 *
 * @param packet Pointer to an array of BotPacket objects, containing velocity and kick data for each bot.
 *
 * @see https://protobuf.dev/getting-started/cpptutorial/
 * @see ssl_simulation_robot_control.proto
 */
void Dhanush::send_velocity(BotPacket* packet)
{
    // Create a RobotControl message that will contain commands for multiple bots
    RobotControl robot_control;

    //loops through half of the bots i.e one team
    for (int i = 0; i < TOTAL_BOTS / 2; ++i) {
        //adds a new RobotCommand to the robot_control message
        command = robot_control.add_robot_commands();

        //setting the robot's ID and kick speed
        command->set_id(packet[i].id);
        command->set_kick_speed(packet[i].kick_speed);

        //creating and populating the move command
        RobotMoveCommand* move_command = command->mutable_move_command();
        MoveLocalVelocity* local_vel = move_command->mutable_local_velocity();

        //setting local frame velocities (forward, left, and angular)
        local_vel->set_forward(packet[i].vel_x);
        local_vel->set_left(packet[i].vel_y);
        local_vel->set_angular(packet[i].vel_angular);
    }

    //serialising the RobotControl message to a QByteArray
    QByteArray dgram;
    dgram.resize(robot_control.ByteSize());  // Resize buffer to fit serialized message
    robot_control.SerializeToArray(dgram.data(), dgram.size());

    //sending the datagram to the correct team port (blue or yellow)
    if (packet->is_blue) {
        socket->writeDatagram(dgram, QHostAddress::LocalHost, SSL_SIMULATION_CONTROL_BLUE_PORT);
    } else {
        socket->writeDatagram(dgram, QHostAddress::LocalHost, SSL_SIMULATION_CONTROL_YELLOW_PORT);
    }
}
