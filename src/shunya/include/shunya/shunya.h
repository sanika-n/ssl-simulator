/*
 * The simulator is divided into various components.
 * If you are versed with mythology you may be able to
 * guess each components purpose.
 *
 * Shunya: Setup the initial position of all bots
 * Also used for resetting bot position back to time zero.
 *
 */

#ifndef SHUNYA_H
#define SHUNYA_H

#include "protobuf/ssl_simulation_control.pb.h"
#include <QObject>
#include <QUdpSocket>
#include <QPointF>

class Shunya : public QObject
{
    Q_OBJECT
public:
    explicit Shunya(QObject *parent = nullptr);
    void setBotPosition(google::protobuf::RepeatedPtrField<sslsim::TeleportRobot> *bot, int id, float x, float y, float orientation=0, bool is_blue=true);
    void setBallPosition(google::protobuf::RepeatedPtrField<sslsim::TeleportBall> *ball, float x, float y);
    ~Shunya();

public slots:
    void setup();
    void attack_setup();
    void defense_setup();
    void move_one_bot(int id, QPointF point, bool is_blue);
    void move_ball(QPointF point);
    void onSocketError(QAbstractSocket::SocketError socketError);
    void handleDatagrams();

signals:

private:
    sslsim::SimulatorCommand *command;
    QUdpSocket *socket;
    QHostAddress _addr;
    quint16 _port;
};

#endif // SHUNYA_H
