#include "shunya.h"
#include "core/sslprotocols.h"
#include "protobuf/ssl_simulation_error.pb.h"
#include <QNetworkDatagram>
#define LOG qDebug() << "[shunya] : "


Shunya::Shunya(QObject *parent)
    : QObject{parent},
    socket(new QUdpSocket(this)),
    command(new sslsim::SimulatorCommand())
{

    /*
     * uncomment this for debugging only, for some reason it occupies the whole loop
     */

    // this->_addr.setAddress(SSL_VISION_ADDRESS_LOCALHOST);
    // this->_port = quint16(SSL_SIMULATED_VISION_PORT);
    // // if socket fails to connect
    // connect(socket, &QAbstractSocket::errorOccurred,this, &Shunya::onSocketError);
    // socket->bind(_addr, _port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    // if(socket->state() != QAbstractSocket::BoundState){
    //     LOG << "socket not bound";
    // }
    // // new syntax, do not use SIGNAL() and SLOT()
    // auto success = connect(socket, &QUdpSocket::readyRead, this, &Shunya::handleDatagrams);
    // if(!success){ LOG << socket->errorString();}

}

void Shunya::onSocketError(QAbstractSocket::SocketError socketError)
{
    LOG<<"socket error occured and the error is : "<<socketError;
}

void Shunya::handleDatagrams()
{
    // when data comes in
    while(socket->hasPendingDatagrams()){
        LOG << "simulator error ";
        auto datagram = socket->receiveDatagram();
        auto buffer = datagram.data();

        sslsim::SimulatorResponse error;
        if(error.ParseFromArray(buffer.data(), buffer.size())){
            if(!error.errors().begin()->has_message()){
                break;
            }
            for(auto itr=error.errors().begin(); itr != error.errors().end(); itr++){
                LOG << QString(itr->message().data());
            }
        }
    }
}

Shunya::~Shunya(){
    delete socket;
    delete command;
    command = nullptr;
    socket = nullptr;
}
void Shunya::move_one_bot(int id, QPointF point, bool is_blue)
{
    //creating the message
    command->Clear();
    sslsim::SimulatorControl *control = command->mutable_control();
    auto bot = control->mutable_teleport_robot();

    //initial position of the bots can be set here
    setBotPosition(bot, id, point.x(), point.y(), 0, is_blue);
    QByteArray dgram;
    dgram.resize(command->ByteSize());
    command->SerializeToArray(dgram.data(), dgram.size());

    //sending message
    if (socket->writeDatagram(dgram, QHostAddress::LocalHost, SSL_SIMULATION_CONTROL_PORT) > -1) {
        // LOG << "sent data";
    }

}

void Shunya::setup()
{
    //creating the message
    command->Clear();
    LOG << "setting up";
    sslsim::SimulatorControl *control = command->mutable_control();
    auto bot = control->mutable_teleport_robot();

    //initial position of the bots can be set here
    setBotPosition(bot, 0, 0, 0, 0.0, false);
    setBotPosition(bot, 1, 1,-1, 0.0, false);
    setBotPosition(bot, 2, 1.5,0, 0.0, false);
    setBotPosition(bot, 3, 1.5,1, 0.0, false);
    setBotPosition(bot, 4, 1.5,-1, 0.0, false);
    QByteArray dgram;
    dgram.resize(command->ByteSize());
    command->SerializeToArray(dgram.data(), dgram.size());

    //sending message
    if (socket->writeDatagram(dgram, QHostAddress::LocalHost, SSL_SIMULATION_CONTROL_PORT) > -1) {
        LOG << "sent data";
    }
}

void Shunya::attack_setup()
{
    //creating the message
    command->Clear();
    LOG << "setting up";
    sslsim::SimulatorControl *control = command->mutable_control();
    auto bot = control->mutable_teleport_robot();

    //initial position of the bots can be set here
    setBotPosition(bot, 0, -0.2, 0.2, 0.0, false);
    setBotPosition(bot, 1, 1.7, 1.8, 0.0, false);
    setBotPosition(bot, 2, -4.1, -0.5, 0.0, false);
    setBotPosition(bot, 3, 2.1, -1.2, 0, false);
    setBotPosition(bot, 4, -1.7, -0.5, 0.0, false);
    setBotPosition(bot, 5, -2.1, -1.8, 0.0, false);

    setBotPosition(bot, 0, 1.0, -1.0, 0.0, true);
    setBotPosition(bot, 1, 1.2, 1.8, 0.0, true);
    setBotPosition(bot, 2, -2.6, -1.5, 0.0, true);
    setBotPosition(bot, 3, 0.3, -2.4, 0.0, true);
    setBotPosition(bot, 4, 2.4, -0.7, 0.0, true);
    setBotPosition(bot, 5, 3.8, 0.3, 0.0, true);

    QByteArray dgram;
    dgram.resize(command->ByteSize());
    command->SerializeToArray(dgram.data(), dgram.size());

    //sending message
    if (socket->writeDatagram(dgram, QHostAddress::LocalHost, SSL_SIMULATION_CONTROL_PORT) > -1) {
        LOG << "sent data";
    }
}


void Shunya::defense_setup()
{
    //creating the message
    command->Clear();
    LOG << "setting up";
    sslsim::SimulatorControl *control = command->mutable_control();
    auto bot = control->mutable_teleport_robot();

    //initial position of the bots can be set here
    setBotPosition(bot, 0, -1, -1.5, 0.0, false);
    setBotPosition(bot, 1, -1.2, 0.2, 0.0, false);
    setBotPosition(bot, 2, -4.1, -0.5, 0.0, false);
    setBotPosition(bot, 3, 0.2, -1.2, 0.0, false);
    setBotPosition(bot, 4, -2, 0.7, 0.0, false);
    setBotPosition(bot, 5, 2.2, -1.4, 0.0, false);

    setBotPosition(bot, 0, 1.0, -0.5, 0.0, true);
    setBotPosition(bot, 1, -1.2,-1.8, 0.0, true);
    setBotPosition(bot, 2, -1.5, 1.5, 0.0, true);
    setBotPosition(bot, 3, 1.3, 0.6, 0.0, true);
    setBotPosition(bot, 4, 2.4, 1.2, 0.0, true);
    setBotPosition(bot, 5, 3.8, 0.3, 0.0, true);

    QByteArray dgram;
    dgram.resize(command->ByteSize());
    command->SerializeToArray(dgram.data(), dgram.size());

    //sending message
    if (socket->writeDatagram(dgram, QHostAddress::LocalHost, SSL_SIMULATION_CONTROL_PORT) > -1) {
        LOG << "sent data";
    }
}

void Shunya::setBallPosition(google::protobuf::RepeatedPtrField<sslsim::TeleportBall> *ball, float x, float y)
{
    auto ball_pos = ball->Add();
    ball_pos->set_x(x);
    ball_pos->set_y(y);
    ball_pos->set_by_force(true);
}


void Shunya::setBotPosition(google::protobuf::RepeatedPtrField<sslsim::TeleportRobot> *bot, int id, float x, float y, float orientation, bool is_blue)
{
    auto bot_pos = bot->Add();
    gameController::BotId *bot_id = bot_pos->mutable_id();
    if(is_blue){
        bot_id->set_team(gameController::BLUE);
    }else{
        bot_id->set_team(gameController::YELLOW);
    }
    LOG << bot_id->team();
    bot_id->set_id(id);
    //we cannot pass partial positions as of now, need to give all values
    bot_pos->set_x(x);
    bot_pos->set_y(y);
    bot_pos->set_present(true);
    bot_pos->set_by_force(true);
    bot_pos->set_orientation(orientation);

}

double Shunya::setOrientation(){

}
