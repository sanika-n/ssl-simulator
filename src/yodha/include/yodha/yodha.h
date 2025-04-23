/*
 * The simulator is divided into various components.
 * If you are versed with mythology you may be able to
 * guess each components purpose.
 *
 * Yodha: Warriors in our epic.
 * frontend representation of bot positions.
 */

#ifndef YODHA_H
#define YODHA_H

#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>

class BlueBotGraphics;
class YellowBotGraphics;

class BlueBot{
public:
    BlueBot(){};
    BlueBot(QGraphicsScene *scene, QGraphicsScene *scene_hotmap, QPointF &&point, float orientation,int id);
    float getx(){ return x; }
    float gety(){ return y; }
    QPointF mapFromScene(float x, float y){ return body_graphics->mapFromScene(x, y); }
    void updatePosition(const QPointF &&point, float orientation);
    int id;
private:
    class BlueBotGraphics : public QGraphicsPathItem{
    public:
        BlueBotGraphics(){};
        BlueBotGraphics(int id): id(id){};
        ~BlueBotGraphics(){};
        BlueBotGraphics(QPainterPath &path, int id):QGraphicsPathItem(path), id(id){};
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    protected:
        int id;
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    };
    float x, y, orientation;
    BlueBotGraphics *body_graphics=nullptr, *body_graphics_hotmap=nullptr;

};

class YellowBot{
public:
    YellowBot(){};
    YellowBot(QGraphicsScene *scene, QGraphicsScene *scene_hotmap, QPointF &&point, float orientation,int id);
    float getx(){ return x; }
    float gety(){ return y; }
    QPointF mapFromScene(float x, float y){ return body_graphics->mapFromScene(x, y); }
    void updatePosition(const QPointF &&point, float orientation);
    int id;
private:
    class YellowBotGraphics : public QGraphicsPathItem{
    public:
        YellowBotGraphics(){};
        YellowBotGraphics(int id): id(id){};
        YellowBotGraphics(QPainterPath &path, int id):QGraphicsPathItem(path), id(id){};
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    protected:
        int id;
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;
    };
    YellowBotGraphics *body_graphics, *body_graphics_hotmap=nullptr;
    float x, y, orientation;
};


class Ball {
public:
    Ball(QColor color, float radius);
    Ball(QPointF pos, QGraphicsScene *scene, QGraphicsScene *scene_hotmap);
    void updatePosition(QPointF pos);  // Update ball's position
    QPointF getPosition() { return position; }
    float getx() { return position.x(); }
    float gety() { return position.y(); }

private:
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
