/*
 * The simulator is divided into various components.
 * If you are versed with mythology you may be able to
 * guess each components purpose.
 *
 * Drona: The strategy module of the simulator
 *  This is where our next actions are determined.
 */

#ifndef DRONA_H
#define DRONA_H

// #define SIMULATOR_MODE


//include necessary headers for bot control and simulator components, protobuf message formats and Qt networking and threading
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



//enum to store team color
enum Team{
    BLUE = 0,
    YELLOW = 1
};

//declare botpacket class
class BotPacket;

/**
 * @class Drona
 * @brief Main class for strategy logic and command transmission.
 *
 * Drona coordinates the motion of bots by communicating with Dhanush.
 * It stores game state information and can move individual bots to given positions.
 */
class Drona : public QObject
{
	Q_OBJECT
public:
    explicit Drona(QObject *parent = 0);
    void moveToPosition(int id, float x, float y, int team, BotPacket *packet); //function to move to a position
    void setPlayers(std::shared_ptr<std::vector<BlueBot>> pandav, std::shared_ptr<std::vector<YellowBot>> kaurav); //stores the set of blue and yellow bots
    void setBall(std::shared_ptr<Ball> ball); //sets ball location
    ~Drona(); //drona deconstructor
    
private:

    /**
     * @brief Bot constructor that stores bot position an color
     */
    struct Bot {
        int x, y;
        Bot() = default;
        Bot(int x, int y, bool is_blue=false): x(x), y(y), is_blue(is_blue){};
        bool is_blue;
    };
    QThread sender_thread;  //thread for running Dhanush
    Dhanush *sender;  //creating a sender of type Dhanush
    BotPacket *m_packet; //bot packet for velocity commands
#if defined SIMULATOR_MODE
    BotPacket *m_blue_packet; //blue team packet(only for sim)
    BotPacket *m_yellow_packet; //yellow team packet(only for sim)
#endif
    std::shared_ptr<std::vector<BlueBot>> pandav;  //blue bots
    std::shared_ptr<std::vector<YellowBot>> kaurav; //yellow bots
    std::shared_ptr<Ball> ball; //ball
    std::vector<QPointF> vertices; //vertices
    bool has_state_; //flag to indicate if state is initialised
/**
 * signals that send info to slots, which react to them
 * @param send
*/
signals:
    void send(BotPacket* packet); //signal of that sends packets containing velocity info
    void draw_graph(std::vector<QPointF> *vertices); //signal to draw UI
public slots:
    void handleState(QByteArray *buffer); //pointer to QByteArraybuffer that contains state data
};
/**
 * @brief HotMap class that is used to visualise formations involving all bots and the ball, based on current state
 * stores all relevant sharepointers and the scene
 */
class HotMap
{
public:
    std::shared_ptr<std::vector<BlueBot>> scene_pandav; //bluebots
    std::shared_ptr<std::vector<YellowBot>> scene_kaurav; //yellowbots
    std::shared_ptr<Ball> scene_ball; //ball
    std::shared_ptr<std::vector<Mantri>> scene_mantri;
    SSL_WrapperPacket state; //state

    /**
     * @brief HotMap constructor to initialise all scene elements and state
     */

    HotMap(std::shared_ptr<std::vector<BlueBot>> scene_pandav, //blue bots
           std::shared_ptr<std::vector<YellowBot>> scene_kaurav, //yellow bots
           std::shared_ptr<Ball> scene_ball, //ball
           std::shared_ptr<std::vector<Mantri>> scene_mantri,
           SSL_WrapperPacket state) //present state
        : scene_pandav(scene_pandav), scene_kaurav(scene_kaurav),
        scene_ball(scene_ball), scene_mantri(scene_mantri), state(state) {}

    //function to reset hotmap
    void setHotMap();
};

#endif // DRONA_H
