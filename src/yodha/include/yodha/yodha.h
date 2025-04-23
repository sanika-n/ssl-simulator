#ifndef YODHA_H
#define YODHA_H
/**
 * @file yodha.h
 * @brief Defines the classes bluebot, bluebotgraphics, yellowbot, yellowbotgraphics and the ball
 *
 * This file contains all the definition regarding the various bots and their graphical aspects
 */

#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>
#include <QGamepad>
#include <QTimer>
#include <QGamepadManager>
class BlueBotGraphics;
class YellowBotGraphics;

/**
 * @class BlueBot
 * @brief Defines how to make the blue robots (its structure) and functions to update its positions
 */
class BlueBot{
public:
    /**
     * Initializes the class
     */
    BlueBot(){};
    /**
     * @brief Makes the bot's structure (physical aspects) and adds the hotmaps for the bot
     * @param *scene The pointer referring to the larger scene
     * @param *scene_hotmap The pointer referring to the large scene's hotmap
     * @param &&point Refers to the adress of the adress of the point
     * @param orientation Refers to the orientation that the bot has to take
     * @param id Refers to the bot's id
     */
    BlueBot(QGraphicsScene *scene, QGraphicsScene *scene_hotmap, QPointF &&point, float orientation,int id);
    /**
     * @brief Gets the x value of the bot
     * @return Returns the x position of the bot
     */
    float getx(){ return x; }
    /**
     * @brief Gets the y value of the bot
     * @return Returns the y position of the bot
     */
    float gety(){ return y; }
    /**
     * @brief Maps from the coordinates of the scene to the viewport
     * @param x The x value of the position of the bot
     * @param y The y value of the position of the bot
     * @return Returns the mapping to the viewport
     */
    QPointF mapFromScene(float x, float y){ return body_graphics->mapFromScene(x, y); }
    /**
     * @brief Updates position of the bots
     * @param &&point Adress of the adress of the point
     * @param orientation Refers to the orientation of the bot in the new position
     */
    void updatePosition(const QPointF &&point, float orientation);
    /**
     * A declaration of the typedef of id as int
     */
    int id;
private:
    /**
     * @class BlueBotGraphics
     * @brief Defines the functions for colouring the bot and its motion with the keyboard and mouse
     */
    class BlueBotGraphics : public QGraphicsPathItem{
    public:
        /**
         * Constructs the class, give the bot the id and deconstructs it
         */
        BlueBotGraphics(){};
        BlueBotGraphics(int id): id(id){};
        ~BlueBotGraphics(){};
        /**
         * @brief Declares the various parameters needed for the bluebot graphics
         * @param &path Refers to the adress of the path
         * @param id Refers to the id of the bot
         */
        BlueBotGraphics(QPainterPath &path, int id):QGraphicsPathItem(path), id(id){};
        /**
         * @brief Function to draw the path and set the color of the bot
         * @param *painter Refers to the value of the painter pointer
         * @param *option Refers to the value of the option pointer inheriting from the class QStyle
         * @param *widget Refers to the value of the widget pointer initialized to nullptr
         */
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    protected:
        /**
         * Typedef of id is declared
         */
        int id;
        /**
         * @brief Defines the function describing what happens when mouse is clicked or moved
         * @param *event Refers to the value of the event pointer
         */
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        /**
         * @brief Defines the function describing what happens when some key is pressed
         * @param *event Refers to the value of the event pointer
         */
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    };
    /**
     * Typedefs of x,y and orientation along with initialzation of the body_graphics and its hotmap to nullptr
     */
    float x, y, orientation;
    BlueBotGraphics *body_graphics=nullptr, *body_graphics_hotmap=nullptr;

};

class RobotSignalEmitter : public QObject {
    Q_OBJECT
public:
    RobotSignalEmitter(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void robotRightClicked(int id, QPointF position, float orientation);
};


/**
 * @class YellowBot
 * @brief Defines how to make the yellow robots (its structure) and functions to update its positions
 *
 * Every function defined inside is the same as that of the blue bots
 */
 
class YellowBot{
    public:
        YellowBot(){};
        YellowBot(QGraphicsScene *scene, QGraphicsScene *scene_hotmap, QPointF &&point, float orientation,int id);
        float getx(){ return x; }
        float gety(){ return y; }
        QPointF mapFromScene(float x, float y){ return body_graphics->mapFromScene(x, y); }
        void updatePosition(const QPointF &&point, float orientation);
        int id;
        RobotSignalEmitter* getSignalEmitter() const { return signalEmitter; }
    
    private:
        RobotSignalEmitter *signalEmitter = nullptr; 
        class YellowBotGraphics : public QGraphicsPathItem{
        public:
            YellowBotGraphics():signalEmitter(nullptr){};
            YellowBotGraphics(int id, RobotSignalEmitter *emitter ): id(id), signalEmitter(emitter){};
            YellowBotGraphics(QPainterPath &path, int id, RobotSignalEmitter *emitter):QGraphicsPathItem(path), id(id), signalEmitter(emitter){
                setFlag(QGraphicsItem::ItemIsFocusable);
            };
            void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
            void focusInEvent(QFocusEvent *event) override;
            void focusOutEvent(QFocusEvent *event) override;
        protected:
            int id;
            RobotSignalEmitter *signalEmitter = nullptr;
            void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
            void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
            void keyPressEvent(QKeyEvent *event) override;
            QGamepad *gamepad = nullptr;
            QTimer *gamepadTimer = nullptr;
            void handleGamepadInput();
        };
        YellowBotGraphics *body_graphics, *body_graphics_hotmap=nullptr;
        float x, y, orientation;
};


/**
 * @class Ball
 * @brief Updates tthe posiiton of the ball and deals with its vvarious graphical aspects
 */
class Ball{

public:
    /**
     * @brief Initializes the various parameters with their typedef
     * @param color Refers to the color
     * @param radius Refers to the radius
     * @param pos Refers to the position
     * @param *scene Refers to the value of the pointer scene
     * @param *scene_hotmap Refers to the value of the pointer scene_hotmap
     */
    Ball(QColor color, float radius);
    Ball(QPointF pos, QGraphicsScene *scene, QGraphicsScene *scene_hotmap);

    /**
     * @brief Function to update the position of the ball
     * @param pos Takes the position of the ball as parameter
     */
    void updatePosition(QPointF pos);
    /**
     * @brief Function to get the position of the ball
     * @return Returns the position of the ball
     */
    QPointF getPosition(){ return position; }
    /**
     * @brief Function to get the x posiiton of the ball
     * @return Returns the x position of the ball
     */
    float getx(){ return position.x(); }
    /**
     * @brief Function to get the y posiiton of the ball
     * @return Returns the y position of the ball
     */
    float gety(){ return position.y(); }

private:
    /**
     * Declares various parameters with their typedefs
     */
    QColor color;
    float radius;
    QPointF position;  // Ball's position

    class BallGraphics : public QGraphicsEllipseItem {
    public:
        BallGraphics(){};

    protected:
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    };

    BallGraphics *graphics = nullptr;  // For regular scene
    BallGraphics *graphics_hotmap = nullptr;  // For hotmap scene
};


#endif // YODHA_H
