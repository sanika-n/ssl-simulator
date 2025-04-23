#include "yodha.h"
#include "shunya/shunya.h"
#include <QtMath>
#include <QDebug>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#define LOG qDebug() << "[yodha] : "
#define ROBOT_RADIUS 10
float SUBTEND_ANGLE=30;

bool left_click_mode = true;

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

void YellowBot::YellowBotGraphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::yellow);
    painter->drawPath(path());
}

void YellowBot::YellowBotGraphics::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        left_click_mode = true;
    }
    else if (event->button() == Qt::RightButton) {
        left_click_mode = false;
    }
}

void YellowBot::YellowBotGraphics::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // should I really use shunya here? seems fine for now
    Shunya temp;
    // LOG << "item frame : " << event->lastPos();
    // LOG << "qt based transform : " << mapToScene(event->lastPos());
    // LOG << "my transform : " << QPointF(pos().x() - event->lastPos().y() ,event->lastPos().x() + pos().y());
    if (left_click_mode) temp.move_one_bot(id, transformFromScene(mapToScene(event->lastPos())), false, true);
    else temp.move_one_bot(id, transformFromScene(mapToScene(event->lastPos())), false, false);

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
    if (event->button() == Qt::LeftButton) {
        left_click_mode = true;
    }
    else if (event->button() == Qt::RightButton) {
        left_click_mode = false;
    }

    // I don't know from where this below code came
    // I'm not deleteing it in case someone needs it

    // sslsim::TeleportRobot teleport;
    // teleport.mutable_id()->set_id(id);  // bot ID (uint32_t)
    // teleport.mutable_id()->set_team(gameController::Team::BLUE); // or YELLOW

    // // Convert simulator coordinates to SSL-Vision frame
    // QPointF targetSimPos(1.0, 2.0);  // your desired x, y in sim coords (in meters)
    // coordinates::toVision(targetSimPos, teleport);  // fills teleport.x(), teleport.y()

    // teleport.set_v_x(0);  // optional: stop the bot
    // teleport.set_v_y(0);
    // teleport.set_by_force(true);  // if needed to override dribbling etc.
}

void BlueBot::BlueBotGraphics::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Shunya temp;
    // LOG << "item frame : " << event->lastPos();
    // LOG << "qt based transform : " << mapToScene(event->lastPos());
    // LOG << "my transform : " << QPointF(pos().x() - event->lastPos().y() ,event->lastPos().x() + pos().y());
    if (left_click_mode) temp.move_one_bot(id, transformFromScene(mapToScene(event->lastPos())), true, true);
    else temp.move_one_bot(id, transformFromScene(mapToScene(event->lastPos())), true, false);
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
