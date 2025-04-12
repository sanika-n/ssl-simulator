#include "dhanush.h"  //include header file
#include "core/sslprotocols.h"
#define LOG qDebug() << "[dhanush] : " //macro for logging
// this method is not ideal, need to think of a better way to get total bots and the format of the game,
// probably some message from the simulator

//definiing total number of bots in the simulation
#define TOTAL_BOTS 10
using namespace sslsim;

Dhanush::Dhanush(): //constructor for dhanush class
    socket(new QUdpSocket(this)) //creating a new QUdpsocket with parent "this"
{

}


Dhanush::~Dhanush(){ //destructor definition
    delete socket;  //destructor deletes the socket
}


/**
 * @brief Prepares and sends velocity packets
 * @param packet
 *
 * Creates sslsim::RobotControl which is a repeated pointer.
 * Creates and sets fields for an sslsim:RobotCommand for each bot. Velocity is given in local frame NOT in global frame.
 * These are sent to SSL_SIMULATION_CONTROL_BLUE_PORT through QUDP Datagrams
 *
 * @see https://protobuf.dev/getting-started/cpptutorial/
 * @see ssl_simulation_robot_control.proto
 *
 */
void Dhanush::send_velocity(BotPacket* packet)
{
    // this can lead to race conditions since memory is shared between two threads.
    // cannot provide local copy since QObjects are non-copyable
    // In practice, it is rare since the signal is emmitted at 100 Hz and this function should
    // finish much faster

    RobotControl robot_control; //create robot control object
    //preparing packet
    //loop is used to format and process a packet before sending.
    for(int i=0;i < TOTAL_BOTS/2; ++i){
        command = robot_control.add_robot_commands(); //creating a command variable
        command->set_id(packet[i].id);  //setting id and kick speed
        command->set_kick_speed(packet[i].kick_speed);
        RobotMoveCommand *move_command = command->mutable_move_command();  //creating a move command
        MoveLocalVelocity *local_vel = move_command->mutable_local_velocity();
        local_vel->set_forward(packet[i].vel_x); //taking velocity values from the packet and adding them to the command
        local_vel->set_left(packet[i].vel_y);
        local_vel->set_angular(packet[i].vel_angular);

    }

    // sending packets

    //serialise the command to a qbyte array
    QByteArray dgram;
    dgram.resize(robot_control.ByteSize()); //resize array to hold serialised data
    robot_control.SerializeToArray(dgram.data(), dgram.size()); //serialise and add data to array

    //check if packet is meant for a blue or yellow bot and publish to the appropriate port
    if(packet->is_blue){
        if (socket->writeDatagram(dgram, QHostAddress::LocalHost, SSL_SIMULATION_CONTROL_BLUE_PORT) > -1) {
            // for logging purposes
        }
    }else{
        if (socket->writeDatagram(dgram, QHostAddress::LocalHost, SSL_SIMULATION_CONTROL_YELLOW_PORT) > -1) {
            // for logging purposes
        }
    }
}
