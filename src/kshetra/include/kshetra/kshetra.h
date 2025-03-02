/*
 * The simulator is divided into various components.
 * If you are versed with mythology you may be able to
 * guess each components purpose.
 *
 * Kshetra: the ground on which and epic battle unfolds
 * A football playing field.
 */

#ifndef KSHETRA_H
#define KSHETRA_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include "yodha/yodha.h"
#include "protobuf/ssl_wrapper.pb.h"
#include "protobuf/ssl_geometry.pb.h"
#include <google/protobuf/repeated_field.h>

// primitive graphics function for later use
// convenience functions that draw some basic stuff
inline QGraphicsEllipseItem* addCircle_(QGraphicsScene *scene, const QPointF &center, int radius, QColor color);
inline QGraphicsLineItem* addLine_(QGraphicsScene *scene, const QPointF &point1, const QPointF &point2, int width);
inline QGraphicsEllipseItem* addArc_(QGraphicsScene *scene, const QPointF center, int radius, int width);

class Kshetra : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Kshetra( QWidget *parent=0);
    inline QPointF transformToScene(QPointF &&point);
    void setPlayers(std::shared_ptr<std::vector<BlueBot>> pandav,std::shared_ptr<std::vector<YellowBot>> kaurav);
    void setBall(std::shared_ptr<Ball> ball);
    void setGround(qint32 length, qint32 width);
    void setFieldLines(const SSL_GeometryFieldSize &field_info);
    const QGraphicsScene *getScene(){ return scene; }
    ~Kshetra();

public slots:
    void handleState(QByteArray *buffer);
    void handleGraph(std::vector<QPointF> *vertices);

private:
    QGraphicsScene *scene;
    QPainter *painter;
    std::shared_ptr<std::vector<BlueBot>> scene_pandav;
    std::shared_ptr<std::vector<YellowBot>> scene_kaurav;
    std::shared_ptr<Ball> scene_ball;
    QVector<QGraphicsLineItem*> lines;

    SSL_WrapperPacket state;
    SSL_GeometryData field_geometry;
    bool has_state_;
    bool ball_init_ = false;
    bool bots_init_ = false;
};

#endif // KSHETRA_H
