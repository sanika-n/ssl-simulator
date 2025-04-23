#include "yodha.h"
#include "shunya/shunya.h"
#include <QtMath>
#include <QDebug>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#define LOG qDebug() << "[yodha] : "
#define ROBOT_RADIUS 10
float SUBTEND_ANGLE=30;

/**
 * @brief Converting to m and changing origin to center of field from top left corner
 *
 * @param point
 * @return QPointF
 *
 * QGraphicsScene : (0,0) top left corner, +y downwards vertical, +x forward horizontal
 * Simulator : (0,0) field center, +y downwards vertical, +x forward horizontal
 */
inline QPointF transformFromScene(const QPointF &&point)
{
    //NOTE : hard coded field length and width!!!
    return QPointF(point.x()/100 - 4.5, point.y()/100 - 3);
}

inline QRectF boundingSquare(const QPointF &center, int half_side)
{
    return QRectF(center.x() - half_side, center.y() - half_side, 2*half_side, 2*half_side);
}

YellowBot::YellowBot(QGraphicsScene *scene, QGraphicsScene *scene_hotmap, QPointF &&point, float orientation, int id):
    id(id),
    x(point.x()),
    y(point.y()),
    orientation(orientation)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QRectF bounding_rect = QRectF(-ROBOT_RADIUS, -ROBOT_RADIUS, 2*ROBOT_RADIUS, 2*ROBOT_RADIUS);
    path.arcTo(bounding_rect, SUBTEND_ANGLE, 360-2*SUBTEND_ANGLE);
    path.closeSubpath();
    path.moveTo(ROBOT_RADIUS*cos(qDegreesToRadians(SUBTEND_ANGLE)), ROBOT_RADIUS*sin(qDegreesToRadians(SUBTEND_ANGLE)));
    path.lineTo(ROBOT_RADIUS*cos(qDegreesToRadians(SUBTEND_ANGLE)), -ROBOT_RADIUS*sin(qDegreesToRadians(SUBTEND_ANGLE)));

    body_graphics = new YellowBotGraphics(path, id);
    body_graphics_hotmap = new YellowBotGraphics(path, id);

    body_graphics->setRotation(qRadiansToDegrees(orientation));
    body_graphics_hotmap->setRotation(qRadiansToDegrees(orientation));

    scene->addItem(body_graphics);
    scene_hotmap->addItem(body_graphics_hotmap);

    if(scene->mouseGrabberItem() != nullptr)
        LOG << "mouse accepted";
}
void YellowBot::updatePosition(const QPointF &&point, float orientation)
{
    x = point.x();
    y = point.y();
    this->orientation = orientation;
    //sets the robot at location point with radius and color
    body_graphics->setPos(point);
    body_graphics->setRotation(qRadiansToDegrees(orientation));

    body_graphics_hotmap->setPos(point);
    body_graphics_hotmap->setRotation(qRadiansToDegrees(orientation));
    return;
}

// Add this to yodha.cpp, replacing the existing YellowBot::YellowBotGraphics::focusInEvent,
// focusOutEvent, and handleGamepadInput functions

void YellowBot::YellowBotGraphics::focusInEvent(QFocusEvent* event) {
    Q_UNUSED(event);

    // First check available gamepads
    auto gamepadIds = QGamepadManager::instance()->connectedGamepads();
    qDebug() << "[yodha] : Available gamepads:" << gamepadIds;

    if (gamepadIds.isEmpty()) {
        qDebug() << "[yodha] : No gamepads detected!";
        // Create gamepad with default index anyway
        gamepad = new QGamepad(0);
    } else {
        // Use the first available gamepad
        gamepad = new QGamepad(gamepadIds.first());
        qDebug() << "[yodha] : Using gamepad with ID:" << gamepadIds.first();
    }

    qDebug() << "[yodha] : Gamepad connected status:" << gamepad->isConnected();

    // Set up connections to monitor gamepad buttons (helpful for debugging)
    QObject::connect(gamepad, &QGamepad::buttonAChanged, [](bool pressed) {
        qDebug() << "[yodha] : Button A:" << pressed;
    });

    QObject::connect(gamepad, &QGamepad::buttonBChanged, [](bool pressed) {
        qDebug() << "[yodha] : Button B:" << pressed;
    });

    QObject::connect(gamepad, &QGamepad::connectedChanged, [](bool connected) {
        qDebug() << "[yodha] : Gamepad connection changed to:" << connected;
    });

    // Set up gamepad timer
    if (!gamepadTimer) {
        gamepadTimer = new QTimer();
        QObject::connect(gamepadTimer, &QTimer::timeout, [this]() {
            this->handleGamepadInput();
        });
        gamepadTimer->start(16);
    }
}

void YellowBot::YellowBotGraphics::focusOutEvent(QFocusEvent* event) {
    Q_UNUSED(event);

    if (gamepadTimer) {
        gamepadTimer->stop();
        delete gamepadTimer;
        gamepadTimer = nullptr;
    }

    if (gamepad) {
        delete gamepad;
        gamepad = nullptr;
    }
}

// Replace the handleGamepadInput function with this improved version:

void YellowBot::YellowBotGraphics::handleGamepadInput() {
    if (!gamepad || !gamepad->isConnected()) {
        return;
    }

    float dx = gamepad->axisLeftX();  // -1 to 1
    float dy = gamepad->axisLeftY();  // -1 to 1
    double r2Value = gamepad->buttonR2();
    // Add a small deadzone to ignore small movements
    if (std::abs(dx) < 0.15) dx = 0;
    if (std::abs(dy) < 0.15) dy = 0;

    if (std::abs(dx) > 0 || std::abs(dy) > 0) {
        // Scale movement
        float speed = 15.0;
        if (r2Value == 1){
            speed = 50.0;
        }
        QPointF newPos = pos() + QPointF(dx * speed, dy * speed);

        // Just update the visual position directly
        //setPos(newPos);

        Shunya temp;
        // Fix: For YellowBot, is_blue should be FALSE, not true
        QPointF tempPos = newPos;
        temp.move_one_bot(id, transformFromScene(std::move(tempPos)), false);

        qDebug() << "[yodha] : Yellow bot" << id << "moved to" << newPos;
    }

    bool previousL1State = false;

    // Inside your function:
    double l1Value = gamepad->buttonL1();
    bool currentL1State = (l1Value == 1);

    if (currentL1State && !previousL1State) {
        id = (id + 1) % 6;
        qDebug() << "[yodha] : ID switched to" << id;
    }

    previousL1State = currentL1State;
}



void YellowBot::YellowBotGraphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::yellow);
    painter->drawPath(path());
}

void YellowBot::YellowBotGraphics::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setFocus();
}

void YellowBot::YellowBotGraphics::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // should I really use shunya here? seems fine for now
    Shunya temp;
    // LOG << "item frame : " << event->lastPos();
    // LOG << "qt based transform : " << mapToScene(event->lastPos());
    // LOG << "my transform : " << QPointF(pos().x() - event->lastPos().y() ,event->lastPos().x() + pos().y());
    temp.move_one_bot(id, transformFromScene(mapToScene(event->lastPos())), false);
}
BlueBot::BlueBot(QGraphicsScene *scene, QGraphicsScene *scene_hotmap, QPointF &&point, float orientation, int id):
    id(id),
    x(point.x()),
    y(point.y()),
    orientation(orientation)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QRectF bounding_rect = QRectF(-ROBOT_RADIUS, -ROBOT_RADIUS, 2*ROBOT_RADIUS, 2*ROBOT_RADIUS);
    path.arcTo(bounding_rect, SUBTEND_ANGLE, 360-2*SUBTEND_ANGLE);
    path.closeSubpath();
    path.moveTo(ROBOT_RADIUS*cos(qDegreesToRadians(SUBTEND_ANGLE)), ROBOT_RADIUS*sin(qDegreesToRadians(SUBTEND_ANGLE)));
    path.lineTo(ROBOT_RADIUS*cos(qDegreesToRadians(SUBTEND_ANGLE)), -ROBOT_RADIUS*sin(qDegreesToRadians(SUBTEND_ANGLE)));

    body_graphics = new BlueBotGraphics(path, id);
    body_graphics->setRotation(qRadiansToDegrees(orientation));

    body_graphics_hotmap = new BlueBotGraphics(path, id);
    body_graphics_hotmap->setRotation(qRadiansToDegrees(orientation));

    scene->addItem(body_graphics);
    scene_hotmap->addItem(body_graphics_hotmap);

}
void BlueBot::updatePosition(const QPointF &&point, float orientation)
{
    x = point.x();
    y = point.y();
    this->orientation = orientation;
    //sets the robot at location point with radius and color
    body_graphics->setPos(point);
    body_graphics->setRotation(qRadiansToDegrees(orientation));

    body_graphics_hotmap->setPos(point);
    body_graphics_hotmap->setRotation(qRadiansToDegrees(orientation));
    return;
}

void BlueBot::BlueBotGraphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::blue);
    painter->drawPath(path());
}

void BlueBot::BlueBotGraphics::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void BlueBot::BlueBotGraphics::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Shunya temp;
    // LOG << "item frame : " << event->lastPos();
    // LOG << "qt based transform : " << mapToScene(event->lastPos());
    // LOG << "my transform : " << QPointF(pos().x() - event->lastPos().y() ,event->lastPos().x() + pos().y());
    temp.move_one_bot(id, transformFromScene(mapToScene(event->lastPos())), true);
}

Ball::Ball(QColor color, float radius):
    color(color),
    radius(radius)
{}

Ball::Ball(QPointF pos, QGraphicsScene *scene, QGraphicsScene *scene_hotmap):
    position(pos),
    color(Qt::black),
    radius(5)
{
    //the coordinate system of QGraphicsScene is: top-left corner as origin and an inverted y-axis
    QRectF bounding_rect = QRectF(pos.x() - radius, pos.y() - radius, 2*radius, 2*radius);
    graphics = scene->addEllipse(bounding_rect, QPen(), QBrush(color));
    graphics_hotmap = scene_hotmap->addEllipse(bounding_rect, QPen(), QBrush(color));
}

void Ball::updatePosition(QPointF pos)
{
    if(graphics == nullptr || graphics_hotmap == nullptr){
        throw std::invalid_argument("ball not added to scene!");
    }
    position = pos;
    graphics->setRect(boundingSquare(pos, radius));
    graphics_hotmap->setRect(boundingSquare(pos, radius));
}

void YellowBot::YellowBotGraphics::keyPressEvent(QKeyEvent *event)
{
    // TODO: Handle keyboard commands to move a selected bot
}
