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
    signalEmitter = new RobotSignalEmitter();
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QRectF bounding_rect = QRectF(-ROBOT_RADIUS, -ROBOT_RADIUS, 2*ROBOT_RADIUS, 2*ROBOT_RADIUS);
    path.arcTo(bounding_rect, SUBTEND_ANGLE, 360-2*SUBTEND_ANGLE);
    path.closeSubpath();
    path.moveTo(ROBOT_RADIUS*cos(qDegreesToRadians(SUBTEND_ANGLE)), ROBOT_RADIUS*sin(qDegreesToRadians(SUBTEND_ANGLE)));
    path.lineTo(ROBOT_RADIUS*cos(qDegreesToRadians(SUBTEND_ANGLE)), -ROBOT_RADIUS*sin(qDegreesToRadians(SUBTEND_ANGLE)));

    body_graphics = new YellowBotGraphics(path, id, signalEmitter);
    body_graphics_hotmap = new YellowBotGraphics(path, id, signalEmitter);

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

    // Initialize SDL if not already initialized (with gamecontroller subsystem)
    if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) == 0) {
        if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
            qDebug() << "[yodha] : SDL could not initialize! SDL Error:" << SDL_GetError();
            return;
        }
    }

    // Check for available controllers
    int numJoysticks = SDL_NumJoysticks();
    qDebug() << "[yodha] : Available controllers:" << numJoysticks;

    // Try to open the first available controller
    for (int i = 0; i < numJoysticks; i++) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
            if (controller) {
                controllerIndex = i;
                qDebug() << "[yodha] : Controller connected:" << SDL_GameControllerName(controller);
                break;
            } else {
                qDebug() << "[yodha] : Could not open controller! SDL Error:" << SDL_GetError();
            }
        }
    }

    // Set up gamepad timer for polling SDL events
    if (!gamepadTimer) {
        gamepadTimer = new QTimer();
        QObject::connect(gamepadTimer, &QTimer::timeout, [this]() {
            this->handleGamepadInput();
        });
        gamepadTimer->start(16); // Poll at approximately 60Hz
    }
}

void YellowBot::YellowBotGraphics::focusOutEvent(QFocusEvent* event) {
    Q_UNUSED(event);

    if (gamepadTimer) {
        gamepadTimer->stop();
        delete gamepadTimer;
        gamepadTimer = nullptr;
    }

    if (controller) {
        SDL_GameControllerClose(controller);
        controller = nullptr;
        controllerIndex = -1;
    }
}

void YellowBot::YellowBotGraphics::handleGamepadInput() {
    if (!controller) {
        return;
    }

    // Process SDL events to update controller state
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_CONTROLLERDEVICEREMOVED &&
            event.cdevice.which == controllerIndex) {
            qDebug() << "[yodha] : Controller disconnected";
            SDL_GameControllerClose(controller);
            controller = nullptr;
            controllerIndex = -1;
            return;
        }
    }

    // Get joystick values (SDL axes range from -32768 to 32767)
    // Convert to -1 to 1 range to match Qt behavior
    float dx = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) / 32767.0f;
    float dy = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) / 32767.0f;
    float rx = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) / 32767.0f;
    float ry = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) / 32767.0f;

    qDebug() << "[yodha] : Right stick values - rx:" << rx << "ry:" << ry;
    qDebug() << "[yodha] : Left stick values - dx:" << dx << "dy:" << dy;

    // Get R2 trigger value (0 to 32767, normalize to 0-1)
    double r2Value = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) / 32767.0f;

    // Add a small deadzone to ignore small movements
    if (std::abs(dx) < 0.15) dx = 0;
    if (std::abs(dy) < 0.15) dy = 0;

    // Check if right stick is actively being used
    bool rightStickActive = (std::abs(rx) > 0.15 || std::abs(ry) > 0.15);

    // Calculate orientation from right joystick if active
    double angleRadians;
    if (rightStickActive) {
        angleRadians = qAtan2(-ry, rx);
        qDebug() << "[yodha] : Right stick active - rx:" << rx << "ry:" << ry
                 << "angle (degrees):" << qRadiansToDegrees(angleRadians);
    } else {
        // Keep current orientation if right stick isn't active
        angleRadians = qDegreesToRadians(rotation());
    }

    // Position update logic
    QPointF newPos = pos();
    bool updatePosition = false;

    // Only update position if left stick is active
    if (std::abs(dx) > 0 || std::abs(dy) > 0) {
        float speed = (r2Value > 0.9) ? 50.0 : 15.0;
        newPos = pos() + QPointF(dx * speed, dy * speed);
        updatePosition = true;
    }

    // Only send updates if there are changes to make
    if (updatePosition || rightStickActive) {
        Shunya temp;
        QPointF tempPos = newPos;

        // Send position and orientation update
        temp.move_one_bot(id, transformFromScene(std::move(tempPos)), false, false, angleRadians);
    }

    // Handle ID switching with L1 button (SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
    static bool previousL1State = false;
    bool currentL1State = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);

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
    // Responsible for deciding whether we want to activate physics mode or teleport mode
    if (event->button() == Qt::LeftButton) {
        left_click_mode = true;
    }
    else if (event->button() == Qt::RightButton) {
        left_click_mode = false;
    }

    // Sends data to the sidebar
    if (event->button() == Qt::RightButton) {
        if (signalEmitter) {
            emit signalEmitter->robotRightClicked(id, this->pos(), this->rotation());
            qDebug() << "Emitted signal from bot id:" << id;
        } else {
            qDebug() << "signalEmitter is null!";
        }
    }

    setFocus();
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
    // Responsible for deciding whether we want to activate physics mode or teleport mode
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
    graphics = new BallGraphics();
    graphics->setRect(bounding_rect); // Set bounding rect for the ball's appearance in the scene
    scene->addItem(graphics);  // Add to the regular scene

    graphics_hotmap = new BallGraphics();
    graphics_hotmap->setRect(bounding_rect);  // Set bounding rect for hotmap appearance
    scene_hotmap->addItem(graphics_hotmap);
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

void Ball::BallGraphics::mouseMoveEvent(QGraphicsSceneMouseEvent *event){

   LOG << "Mouse Move Event Triggered!";
        Shunya temp;
        QPointF newPos = transformFromScene(mapToScene(event->lastPos()));
        temp.move_ball(newPos);

}

void Ball::BallGraphics::mousePressEvent(QGraphicsSceneMouseEvent *event){}
void YellowBot::YellowBotGraphics::keyPressEvent(QKeyEvent *event)
{
    // TODO: Handle keyboard commands to move a selected bot
}
