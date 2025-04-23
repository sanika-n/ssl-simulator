#include "drona.h"
#include "planner.h"
#include <QString>
#include <cmath>
#include <QtMath>
#include <QNetworkDatagram>

#define LOG qDebug() << "[drona] :" ///< Debug macro for logging messages
#define BLUE_BOTS 6                 ///< Total number of blue bots
#define YELLOW_BOTS 6               ///< Total number of yellow bots

// Test stuff (remove this if you want)

using namespace sslsim;

/**
 * @brief Constructor for the Drona class.
 *
 * Initializes the velocity sender (Dhanush), assigns it to a separate thread,
 * and sets up BotPacket memory for controlling the bots.
 *
 * @param parent Parent QObject
 */
Drona::Drona(QObject* parent)
    : QObject(parent), sender(new Dhanush()) {

    sender->moveToThread(&sender_thread);
    connect(this, &Drona::send, sender, &Dhanush::send_velocity);
    sender_thread.setObjectName("sender");
    sender_thread.start();

    m_packet = new BotPacket[BLUE_BOTS];

#if defined(SIMULATOR_MODE)
    m_blue_packet = new BotPacket[BLUE_BOTS];
    m_yellow_packet = new BotPacket[YELLOW_BOTS];
#endif
}

/**
 * @brief Sets the players on both teams.
 *
 * @param pandav Shared pointer to the vector of blue bots
 * @param kaurav Shared pointer to the vector of yellow bots
 */
void Drona::setPlayers(std::shared_ptr<std::vector<BlueBot>> pandav,
                       std::shared_ptr<std::vector<YellowBot>> kaurav) {
    this->pandav = pandav;
    this->kaurav = kaurav;
}

/**
 * @brief Sets the ball object.
 *
 * @param ball Shared pointer to the Ball object
 */
void Drona::setBall(std::shared_ptr<Ball> ball) {
    this->ball = ball;
}

/**
 * @brief Commands a bot to move to a specific (x, y) location using proportional control.
 *
 * Converts the target world coordinates into bot-relative coordinates and calculates
 * appropriate velocities. Populates the BotPacket with these values.
 *
 * @param id ID of the bot
 * @param x Target X coordinate
 * @param y Target Y coordinate
 * @param team Team identifier (BLUE or YELLOW)
 * @param packet Pointer to array of BotPacket to populate
 */
void Drona::moveToPosition(int id, float x, float y, int team, BotPacket *packet) {
    QPointF relative_pos;

    if (team == Team::BLUE) {
        relative_pos = pandav->at(id).mapFromScene(x, y);
        packet[id].is_blue = true;
    } else {
        relative_pos = kaurav->at(id).mapFromScene(x, y);
        packet[id].is_blue = false;
    }

    QPointF err = relative_pos;
    float orientation_err = qAtan2(relative_pos.y(), relative_pos.x());
    orientation_err = relative_pos.y() > 0 ? fabs(orientation_err) : -fabs(orientation_err);

    float dist_err = std::sqrt(err.x() * err.x() + err.y() * err.y());
    float kp = 0.01f;

    float vel_for = dist_err * kp;
    float vel_th = 2 * orientation_err;

    packet[id].id = id;
    packet[id].vel_angular = vel_th;
    packet[id].vel_x = vel_for;
    packet[id].vel_y = 0.0f;
    packet[id].kick_speed = 5.0f;
}

/**
 * @brief Responds to new world state and sends updated velocity packets to bots.
 *
 * This function is triggered by the Vyasa::receivedState signal. It plans paths and
 * uses moveToPosition to generate commands, resets packets, and emits the send signal
 * to Dhanush.
 *
 * @param buffer Unused buffer from world state (placeholder)
 */
void Drona::handleState(QByteArray* buffer) {
    static int counter = 0;

    std::vector<std::pair<double, double>> bot_pos;
    for (int i = 0; i < kaurav->size(); ++i) {
        bot_pos.push_back({kaurav->at(i).getx(), kaurav->at(i).gety()});
    }

    std::pair<double, double> endpt = {0.0f, 100.0f};

    if (counter == 100) {
        vertices = plan_path(bot_pos, endpt, 0);
        counter = 0;
    }

    emit draw_graph(&vertices);

#if defined(SIMULATOR_MODE)
    for (int i = 0; i < BLUE_BOTS; ++i) {
        m_blue_packet[i].id = i;
        m_blue_packet[i].is_blue = true;
        m_blue_packet[i].vel_x = 0.0f;
        m_blue_packet[i].vel_y = 0.0f;
    }

    for (int i = 0; i < YELLOW_BOTS; ++i) {
        m_yellow_packet[i].id = i;
        m_yellow_packet[i].is_blue = false;
        m_yellow_packet[i].vel_x = 0.0f;
        m_yellow_packet[i].vel_y = 0.0f;
    }

    moveToPosition(10, ball->getPosition().x(), ball->getPosition().y(), Team::YELLOW, m_yellow_packet);

    emit send(m_blue_packet);
    emit send(m_yellow_packet);
#else
    for (int i = 0; i < YELLOW_BOTS; ++i) {
        m_packet[i].id = i;
        m_packet[i].is_blue = false;
        m_packet[i].vel_x = 0.0f;
        m_packet[i].vel_y = 0.0f;
        m_packet[i].vel_angular = 0.0f;
    }

    if (!vertices.empty()) {
        moveToPosition(kaurav->front().id, vertices.back().x(), vertices.back().y(), Team::YELLOW, m_packet);
    }

    emit send(m_packet);
#endif
    counter++;
}

/**
 * @brief Destructor for Drona. Cleans up the thread and memory.
 */
Drona::~Drona() {
    delete sender;
    sender_thread.quit();
    sender_thread.wait();
}

/**
 * @brief Sets color values for each HotMap scene element.
 *
 * This is a placeholder for hot map logic. Currently, it sets all color values to 0.
 */
void HotMap::setHotMap() {
    for (int i = 0; i < scene_mantri->size(); ++i) {
        float color_value = 0;

        // Strategy logic for hotmap could be added here

        if (color_value <= 0) color_value = 0;
        scene_mantri->at(i).updateColor(color_value, true);
    }
}

