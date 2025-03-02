#ifndef MANTRI_H
#define MANTRI_H

#include <QGraphicsScene>

class MantriGraphics;

class Mantri{
public:
    Mantri(){};
    Mantri(QGraphicsScene *scene, QPointF &&point, float intensity, float width, float radius);
    void updateColor(float intensity, bool paint);
    float getx(){return x;}
    float gety(){return y;}
private:
    float x, y, radius, width, intensity;
    QGraphicsRectItem *graphics = nullptr;
};

#endif // MANTRI_H
