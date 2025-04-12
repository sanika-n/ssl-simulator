/*
 * The simulator is divided into various components.
 * If you are versed with mythology you may be able to
 * guess each components purpose.
 *
 * Dhanush: runs to the backend to inform the next
 *  bot positions.
 */

#ifndef DHANUSH_H
#define DHANUSH_H


#include "protobuf/ssl_simulation_robot_control.pb.h"
#include <QObject>
#include <QUdpSocket>
#include <QString>
#include <QNetworkDatagram>
#include <google/protobuf/repeated_field.h>

//TODO: Struct for passing velocities together

class BotPacket: public QObject //class that stores motion control data and state data of a bot
{
    Q_OBJECT

public:
    BotPacket(): vel_x(0.0f), vel_y(0.0f), vel_angular(0.0f), kick_speed(0.0f){}; //botpacket constructor, initialises member variables with their types
    BotPacket(int id, bool is_blue): vel_x(0.0f), vel_y(0.0f), vel_angular(0.0f), kick_speed(0.0f), id(id), is_blue(is_blue){};//parametrised constructor, allows initialisation with values.
    ~BotPacket() = default; //destructor to delete the object when it is out of scope
    float vel_x, vel_y, vel_angular; //x, y and angular velocity
    int id; //bot id
    bool is_blue; //true if bot is blue
    float kick_speed; //speed at which bot kicks ball
};

class Dhanush: public QObject  //main class that sends ball velocity updates and controls velocity
{
    Q_OBJECT

public:
    // struct bot_packet{
    //     float vel_x, vel_y, vel_angular;
    //     int id;
    //     bool is_blue;
    // };
    Dhanush();  //initialising dhanush object
    ~Dhanush(); //initialising its destructor

private:
    struct Bot{  //custom structure for storing bot position, id, colour
        int x, y;
        int id;
        Bot(int x, int y,int id, bool is_blue=false): x(x), y(y), is_blue(is_blue), id(id){}; //bot struct parametrised constructor
        bool is_blue;
    };
    sslsim::RobotCommand *command; //initialising pointers
    QUdpSocket* socket;
    QNetworkDatagram datagram;
    QByteArray buffer;
    void moveToPosition(float x, float y); //function to move to a position

public slots:
    // void send_velocity(std::vector<bot_packet> packet);
    void send_velocity(BotPacket* packet); //a velocity slot that responds with a botpacket object.


};

#endif // DHANUSH_H
