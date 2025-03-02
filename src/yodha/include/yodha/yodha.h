#ifndef YODHA_H
#define YODHA_H

#include <QGraphicsScene>
#include <QGraphicsPathItem>
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
        void keyPressEvent() override;
    };
    YellowBotGraphics *body_graphics, *body_graphics_hotmap=nullptr;
    float x, y, orientation;
};

class Ball{
public:
    Ball(QColor color, float radius);
    Ball(QPointF pos, QGraphicsScene *scene, QGraphicsScene *scene_hotmap);
    void updatePosition(QPointF pos);
    QPointF getPosition(){ return position; }
    float getx(){ return position.x(); }
    float gety(){ return position.y(); }
private:
    QColor color;
    float radius;
    QGraphicsEllipseItem *graphics = nullptr;
    QGraphicsEllipseItem *graphics_hotmap = nullptr;
    QPointF position;
};

#endif // YODHA_H
