#include "drona.h"
#include "planner.h"
#include <QString>
#include <cmath>
#include <QtMath>
#include <QNetworkDatagram>
#define LOG qDebug() << "[drona] : "  //macro to format log messages
#define BLUE_BOTS 6 //set number of blue bots
#define YELLOW_BOTS 6 //set number of yellow bots

using namespace sslsim;
/**
 * @brief constructor for Drona class.
 * creates a Dhanush sender object, that sends velocities.
 * creates m_packet to send velocity commands
 * @param parent
 */

Drona::Drona(QObject* parent) : QObject(parent),
    sender(new Dhanush())
{
    //allocating a thread to sender
    sender->moveToThread(&sender_thread);
    connect(this, &Drona::send, sender, &Dhanush::send_velocity);//connecting the sender with drona and dhanush objects

    sender_thread.setObjectName("sender");
    sender_thread.start();
    m_packet = new BotPacket[BLUE_BOTS];
#if defined(SIMULATOR_MODE)
    m_blue_packet = new BotPacket[BLUE_BOTS];
    m_yellow_packet = new BotPacket[YELLOW_BOTS];
#endif
}
/**
 * @brief stores shared pointers to blue and yellow bots
 */
void Drona::setPlayers(std::shared_ptr<std::vector<BlueBot>> pandav, std::shared_ptr<std::vector<YellowBot>> kaurav)
{
    this->pandav = pandav;
    this->kaurav = kaurav;
}

/**
 * @brief stores shared pointers to the ball */
void Drona::setBall(std::shared_ptr<Ball> ball)
{
    this->ball = ball;
}

/**
 * @brief Moves the bot to (x,y) field coordinates using P-control
 *
 * @param id of the bot.
 * @param x
 * @param y
 * @param team - team colour
 *Calculates the relative position of the bot wrt target position and user Proportional Control to change velocity.
 * @see https://doc.qt.io/qt-6/qgraphicsitem.html#mapToScene
 */


void Drona::moveToPosition(int id, float x, float y, int team, BotPacket *packet)
{
    // calculating the x and y velocities
    QPointF relative_pos;
    if(team == Team::BLUE){
        relative_pos = pandav->at(id).mapFromScene(x, y);
        packet[id].is_blue = true;
    }else{
        relative_pos = kaurav->at(id).mapFromScene(x, y);
        packet[id].is_blue = false;
    }

    QPointF err = relative_pos;
    float orientation_err = qAtan2(relative_pos.y(), relative_pos.x());//calculating angle between position and target position using relative position
    orientation_err = relative_pos.y() > 0 ? fabs(orientation_err) : -fabs(orientation_err);
    float dist_err = pow(err.x()*err.x() + err.y()*err.y(), 0.5);//relative distnace magnitude
    float kp = 0.01; //kp for proportional tuning of velocity
    float vel_for = dist_err*kp; //changing velocity gradually
    float vel_th = 2*orientation_err;
    //adding all values to packets
    packet[id].vel_angular = vel_th;
    packet[id].vel_x = vel_for;
    packet[id].vel_y = 0.0f;
    packet[id].id = id;
    packet[id].kick_speed = 5.0f;
}

/**
 * @brief Resets each BotPacket for blue bots, gives new velocity commands and emits packets again.
 *
 * @param buffer
 *
 * @see Vyasa::receivedState
 *
 * Include bot-specific movement commands like moveToPosition here.
 *
 * Called when Vyasa::receivedState signal is emitted
 *
 * Drona::send signal is recived by the slot Dhanush::send_velocity
 *
 */
void Drona::handleState(QByteArray //called when a new state is received. triggers bot movement.*buffer)
{

    //extracts all yellow bot positions
    static int counter = 0;
    std::vector<std::pair<double, double>> bot_pos;
    for(int i=0; i < kaurav->size(); ++i){
        bot_pos.push_back(make_pair(kaurav->at(i).getx(), kaurav->at(i).gety()));
    }

    //plans new paths and has them drawn every 100 calls.
    std::pair<double, double> endpt;
    endpt.first = 0.0f;
    endpt.second = 100.0f;
    // LOG << counter << endl;
    if(counter == 100){
        vertices = plan_path(bot_pos, endpt, 0);
        counter =0;
    }
    emit draw_graph(&vertices);
    // LOG << vertices.size();
    // }
#if defined(SIMULATOR_MODE)
    // reseting packet, will make this better
    for(int i=0; i < BLUE_BOTS; ++i){
        m_blue_packet[i].id = i;
        m_blue_packet[i].is_blue = true;
        m_blue_packet[i].vel_x = 0.0f;
        m_blue_packet[i].vel_y = 0.0f;

    }
    //resetting yellow packets
    for(int i=0; i < YELLOW_BOTS; ++i){
        m_yellow_packet[i].id = i;
        m_yellow_packet[i].is_blue = false;
        m_yellow_packet[i].vel_x = 0.0f;
        m_yellow_packet[i].vel_y = 0.0f;

    }
    //move the bots to predefined positions
    moveToPosition(10, ball->getPosition().x(), ball->getPosition().y(), Team::YELLOW, m_yellow_packet);
    // moveToPosition(0, ball->getPosition().x(), ball->getPosition().y(), Team::YELLOW, m_yellow_packet);
    // moveToPosition(0, ball->getPosition().x(), ball->getPosition().y(), Team::BLUE, m_blue_packet);
    // moveToPosition(1, ball->getPosition().x(), ball->getPosition().y(), Team::BLUE, m_blue_packet);
    //send data
    emit send(m_blue_packet);
    emit send(m_yellow_packet);
#else

    // reseting packet, will make this better
    for(int i=0; i < YELLOW_BOTS; ++i){
        m_packet[i].id = i;
        m_packet[i].is_blue = false;
        m_packet[i].vel_x = 0.0f;
        m_packet[i].vel_y = 0.0f;
        m_packet[i].vel_angular = 0.0f;

    }
    //sends only yellow bots to predefined positions
    if(vertices.size() > 0)moveToPosition(kaurav->front().id, vertices.back().x(), vertices.back().y(), Team::YELLOW, m_packet);
    emit send(m_packet);
#endif //SIMULATOR MODE
    counter++;
}


/**
 * @brief destructor for Drona class, disconnects the tread and cleans up

*/
Drona::~Drona()
{
    delete sender;
    sender_thread.quit();
    sender_thread.wait();
}
/**
 * @brief set hotmap function for hotmap logic */
void HotMap::setHotMap(){
    for(auto i = 0; i < scene_mantri->size(); i++)
    {
        float color_value = 0;
        // Write your strategy code here
	
        if(color_value <= 0) color_value = 0;
        scene_mantri->at(i).updateColor(color_value, true);
    }
}

