/*
 * The simulator is divided into various components.
 * If you are versed with mythology, you may be able to
 * guess each component's purpose.
 *
 * Drona: The strategy module of the simulator.
 *  This is where our next actions are determined.
 */

#ifndef DRONA_H
#define DRONA_H

// #define SIMULATOR_MODE  // Uncomment this to enable simulator mode.

/// Include necessary headers for bot control, simulator components, protobuf message formats, and Qt networking/threading.
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

/// Enum to represent team color.
enum Team {
    BLUE = 0,   ///< Blue team
    YELLOW = 1  ///< Yellow team
};

/// Forward declaration of BotPacket class
class BotPacket;

/**
 * @class Drona
 * @brief The main class for strategy logic and command transmission.
 *
 * Drona coordinates the motion of bots by communicating with Dhanush.
 * It stores the game state information and determines the next actions for the bots.
 */
class Drona : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for Drona class.
     * Initializes necessary components and sets up the thread for Dhanush.
     * 
     * @param parent The parent QObject for this instance.
     */
    explicit Drona(QObject *parent = 0);

    /**
     * @brief Moves the specified bot to the given position.
     * 
     * @param id The ID of the bot to be moved.
     * @param x The target x-coordinate.
     * @param y The target y-coordinate.
     * @param team The team to which the bot belongs (BLUE or YELLOW).
     * @param packet The BotPacket that holds velocity information.
     */
    void moveToPosition(int id, float x, float y, int team, BotPacket *packet);

    /**
     * @brief Sets the blue and yellow bot teams.
     * 
     * @param pandav A shared pointer to the list of blue team bots.
     * @param kaurav A shared pointer to the list of yellow team bots.
     */
    void setPlayers(std::shared_ptr<std::vector<BlueBot>> pandav, std::shared_ptr<std::vector<YellowBot>> kaurav);

    /**
     * @brief Sets the ball's location.
     * 
     * @param ball A shared pointer to the ball object.
     */
    void setBall(std::shared_ptr<Ball> ball);

    /**
     * @brief Destructor for Drona class.
     * Cleans up any resources used by the Drona instance.
     */
    ~Drona();

private:
    /**
     * @brief Bot struct stores bot position and color.
     * Used internally by Drona to manage bot states.
     */
    struct Bot {
        int x, y; ///< The x and y coordinates of the bot.
        bool is_blue; ///< True if the bot belongs to the blue team.
        int id; ///< The bot's unique identifier.

        /**
         * @brief Parameterized constructor for Bot.
         * 
         * @param x The x-coordinate of the bot.
         * @param y The y-coordinate of the bot.
         * @param is_blue True if the bot is blue, otherwise false.
         * @param id The unique ID of the bot.
         */
        Bot(int x, int y, bool is_blue=false, int id=-1)
            : x(x), y(y), is_blue(is_blue), id(id) {}
    };

    QThread sender_thread;  ///< Thread to handle Dhanush communication.
    Dhanush *sender;  ///< Pointer to Dhanush instance for sending bot control data.
    BotPacket *m_packet; ///< Bot packet containing velocity information.

#if defined SIMULATOR_MODE
    BotPacket *m_blue_packet;  ///< Blue team packet (used only in simulator mode).
    BotPacket *m_yellow_packet;  ///< Yellow team packet (used only in simulator mode).
#endif

    std::shared_ptr<std::vector<BlueBot>> pandav;  ///< Shared pointer to the list of blue team bots.
    std::shared_ptr<std::vector<YellowBot>> kaurav; ///< Shared pointer to the list of yellow team bots.
    std::shared_ptr<Ball> ball; ///< Shared pointer to the ball object.
    std::vector<QPointF> vertices;  ///< A list of vertices representing the formation or paths of bots.

    bool has_state_; ///< Flag indicating if the game state has been initialized.

signals:
    /**
     * @brief Signal to send velocity command to bots.
     * 
     * @param packet The BotPacket containing velocity data.
     */
    void send(BotPacket* packet);

    /**
     * @brief Signal to draw the current formation or state on the UI.
     * 
     * @param vertices A list of points representing the formation.
     */
    void draw_graph(std::vector<QPointF> *vertices);

public slots:
    /**
     * @brief Slot to handle game state data.
     * 
     * @param buffer A pointer to a QByteArray containing the state data.
     */
    void handleState(QByteArray *buffer);
};

/**
 * @class HotMap
 * @brief Visualizes the formations involving all bots and the ball based on the current state.
 *
 * HotMap stores all relevant shared pointers (bots, ball, etc.) and provides functionality to reset the visual map.
 */
class HotMap
{
public:
    std::shared_ptr<std::vector<BlueBot>> scene_pandav;  ///< Blue team bots for the scene.
    std::shared_ptr<std::vector<YellowBot>> scene_kaurav; ///< Yellow team bots for the scene.
    std::shared_ptr<Ball> scene_ball; ///< The ball in the scene.
    std::shared_ptr<std::vector<Mantri>> scene_mantri; ///< The Mantri (strategists) in the scene.
    SSL_WrapperPacket state; ///< The current state of the game.

    /**
     * @brief HotMap constructor initializes all scene elements and the state.
     * 
     * @param scene_pandav Shared pointer to blue team bots.
     * @param scene_kaurav Shared pointer to yellow team bots.
     * @param scene_ball Shared pointer to the ball.
     * @param scene_mantri Shared pointer to the strategists (Mantri).
     * @param state The current game state.
     */
    HotMap(std::shared_ptr<std::vector<BlueBot>> scene_pandav,
           std::shared_ptr<std::vector<YellowBot>> scene_kaurav,
           std::shared_ptr<Ball> scene_ball,
           std::shared_ptr<std::vector<Mantri>> scene_mantri,
           SSL_WrapperPacket state)
        : scene_pandav(scene_pandav), scene_kaurav(scene_kaurav),
          scene_ball(scene_ball), scene_mantri(scene_mantri), state(state) {}

    /**
     * @brief Resets the HotMap to a new state.
     */
    void setHotMap();
};

#endif // DRONA_H

