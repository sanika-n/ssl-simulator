#include "mantri.h"
#include "shunya/shunya.h"
#include <QDebug>
#include <QPainter>
#include <QGraphicsRectItem>
#include <QtWidgets>

#define LOG qDebug() << "[yodha] : "

inline QPointF transformFromScene(const QPointF &&point)
{
    // converting to m and changing origin to center of field from top left corner
    // QGraphicsScene : (0,0) top left corner, +y downwards vertical, +x forward horizontal
    // Simulator : (0,0) field center, +y downwards vertical, +x forward horizontal
    //NOTE : hard coded field length and width!!!
    return QPointF(point.x()/100 - 4.5, point.y()/100 - 3);
}

inline QRectF boundingSquare(const QPointF &center, int half_side)
{
    return QRectF(center.x() - half_side, center.y() - half_side, 2*half_side, 2*half_side);
}

Mantri::Mantri(QGraphicsScene *scene, QPointF &&point, float intensity, float width, float radius):
    x(point.x()),
    y(point.y()),
    intensity(intensity),
    width(width),
    radius(radius)
{
    QRectF bounding_rect = QRectF(point.x() - radius, point.y() - radius, 2*radius, 2*radius);
    // Base red color
    QColor baseColor = Qt::yellow;

    // Clamp the value to a reasonable range, e.g., 0 to 200
    float value;

    if(intensity <= 0){
        baseColor = Qt::blue;

        if(intensity < -200){
            value = 200;
        }
        else{
            value = -1*intensity;
        }
    }
    else if(intensity >= 200){
        value = 200;
    }
    else{
        value = intensity;
    }

    // Higher value -> darker shade, lower value -> lighter shade
    QColor adjustedColor;
    if (value > 100) {
        adjustedColor = baseColor.darker(value); // Darker for values > 100
    } else {
        adjustedColor = baseColor.lighter(200 - value); // Lighter for values <= 100
    }

    graphics = scene->addRect(bounding_rect, QPen(Qt::NoPen), QBrush(adjustedColor));
}

void Mantri::updateColor(float intensity, bool paint){
    if(graphics == nullptr){
        throw std::invalid_argument("ball not added to scene!");
    }

    // Base red color
    QColor baseColor = Qt::yellow;

    // Clamp the value to a reasonable range, e.g., 0 to 200
    float value;

    if(intensity <= 0){
        baseColor = Qt::blue;

        if(intensity < -200){
            value = 200;
        }
        else{
            value = -1*intensity;
        }
    }
    else if(intensity >= 200){
        value = 200;
    }
    else{
        value = intensity;
    }

    // Higher value -> darker shade, lower value -> lighter shade
    QColor adjustedColor;

    if (value > 100) {
        adjustedColor = baseColor.darker(value); // Darker for values > 100
    } else {
        adjustedColor = baseColor.lighter(200 - value); // Lighter for values <= 100
    }

    if (paint) graphics->setBrush(QBrush(adjustedColor));
}
