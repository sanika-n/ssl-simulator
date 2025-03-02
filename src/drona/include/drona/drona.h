#ifndef DRONA_H
#define DRONA_H

// #define SIMULATOR_MODE

#include "dhanush.h"
#include "yodha/yodha.h"
#include "yodha/mantri.h"
#include "protobuf/ssl_wrapper.pb.h"
#include "protobuf/ssl_geometry.pb.h"
#include <vector>
#include <QObject>
#include <QUdpSocket>
#include <QString>
#include <QThread>
#include <QNetworkDatagram>

enum Team{
    BLUE = 0,
    YELLOW = 1
};

class BotPacket;

class Drona : public QObject
{
	Q_OBJECT
public:
    explicit Drona(QObject *parent = 0);
    void moveToPosition(int id, float x, float y, int team, BotPacket *packet);
    void setPlayers(std::shared_ptr<std::vector<BlueBot>> pandav, std::shared_ptr<std::vector<YellowBot>> kaurav);
    void setBall(std::shared_ptr<Ball> ball);
    ~Drona();
    
private:
    struct Bot {
        int x, y;
        Bot() = default;
        Bot(int x, int y, bool is_blue=false): x(x), y(y), is_blue(is_blue){};
        bool is_blue;
    };
    QThread sender_thread;
    Dhanush *sender;
    BotPacket *m_packet;
#if defined SIMULATOR_MODE
    BotPacket *m_blue_packet;
    BotPacket *m_yellow_packet;
#endif
    std::shared_ptr<std::vector<BlueBot>> pandav;
    std::shared_ptr<std::vector<YellowBot>> kaurav;
    std::shared_ptr<Ball> ball;
    std::vector<QPointF> vertices;
    bool has_state_;

signals:
    void send(BotPacket* packet);
    void draw_graph(std::vector<QPointF> *vertices);
public slots:
    void handleState(QByteArray *buffer);
};

class HotMap
{
public:
    std::shared_ptr<std::vector<BlueBot>> scene_pandav;
    std::shared_ptr<std::vector<YellowBot>> scene_kaurav;
    std::shared_ptr<Ball> scene_ball;
    std::shared_ptr<std::vector<Mantri>> scene_mantri;
    SSL_WrapperPacket state;

    HotMap(std::shared_ptr<std::vector<BlueBot>> scene_pandav,
           std::shared_ptr<std::vector<YellowBot>> scene_kaurav,
           std::shared_ptr<Ball> scene_ball,
           std::shared_ptr<std::vector<Mantri>> scene_mantri,
           SSL_WrapperPacket state)
        : scene_pandav(scene_pandav), scene_kaurav(scene_kaurav),
        scene_ball(scene_ball), scene_mantri(scene_mantri), state(state) {}

    void setHotMap();
};

#endif // DRONA_H
