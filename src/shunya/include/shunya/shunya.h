#ifndef SHUNYA_H
#define SHUNYA_H
/**
 * @file shunya.h 
 * @brief Defines the shunya class which sets up the playing field
 * 
 * This file contains the definitions for settingup all the players and establishing conenction
 */

#include "protobuf/ssl_simulation_control.pb.h"
#include <QObject>
#include <QUdpSocket>
#include <QPointF>

/**
 * @class Shunya
 * @brief Defines various functions for setup
 * 
 * It defines functions such as setting the position of the bots and balls along with socket errors and handling datgrams.
 */
class Shunya : public QObject
{
    Q_OBJECT
public:
    explicit Shunya(QObject *parent = nullptr);
    /**
     * @brief Sets the bot positions
     * @param *bot The pointer to the bot in protobuf
     * @param id The id of the bots
     * @param x The x position of the bots
     * @param y The y position of the bots
     * @param orientation The orientation fo the bots
     * @param is_blue The boolean for whether a bot is blue or yellow
     */
    void setBotPosition(google::protobuf::RepeatedPtrField<sslsim::TeleportRobot> *bot, int id, float x, float y, float orientation=0, bool is_blue=true);
    /**
     * @brief Sets the ball positions
     * @param *ball The pointer to the ball in protobuf
     * @param id The id of the bots
     * @param x The x position of the bots
     * @param y The y position of the bots
     */
    void setBallPosition(google::protobuf::RepeatedPtrField<sslsim::TeleportBall> *ball, float x, float y);
    ~Shunya();

public slots:
    /**
     * @brief Initializes the positions of the bots
     */
    void setup();
    /**
     * @brief Initializes the positions of the bots in attack setup
     */
    void attack_setup();
    /**
     * @brief Intializes the positions of the bots in defense setup
     */
    void defense_setup();
    /**
     * @brief Moves only one bot during the game
     * @param id The id of the bot
     * @param point The position of the bot
     * @param is_blue The boolean for whether the bot is blue or not
     */
    void move_one_bot(int id, QPointF point, bool is_blue, bool instant_teleportation_ignore_physics = false);
    /**
     * @brief Logs the connection error
     * @param socketError
     */

    void move_ball(QPointF point);

    void onSocketError(QAbstractSocket::SocketError socketError);
    /**
     * @brief Handles the data passing through udp till all the data is receieved and logs the errors in the case that it does not
     */
    void handleDatagrams();

signals:

private:
    /**
     * Initializes the various pointers needed
     */
    sslsim::SimulatorCommand *command;
    QUdpSocket *socket;
    QHostAddress _addr;
    quint16 _port;
};

#endif // SHUNYA_H
