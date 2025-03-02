#ifndef KSHETRA_H
#define KSHETRA_H

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include "yodha/yodha.h"
#include "yodha/mantri.h"
#include "drona/drona.h"
#include "protobuf/ssl_wrapper.pb.h"
#include "protobuf/ssl_geometry.pb.h"
#include <google/protobuf/repeated_field.h>

class Bot;
class Ball;

//primitive graphics function for later use
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
    void viewHotMap();
    const QGraphicsScene *getScene(){
        if(!see_hotmap_) return scene;
        else return scene_hotmap;
    }
    ~Kshetra();

public slots:
    void handleState(QByteArray *buffer);
    void handleGraph(std::vector<QPointF> *vertices);
    void setHotMap();

private:
    QGraphicsScene *scene;
    QGraphicsScene *scene_hotmap;
    QGraphicsScene *scene_hotmap_future;
    QPainter *painter;
    // you want to have the same object and share it with drona and Kshetra right?? who updates it? Kshetra obv but drona also uses it
    // then drona does not need a handleState, does that make sense? yeah ig, you would want the updation to happen and you just have a pointer
    // to the bots
    std::shared_ptr<std::vector<BlueBot>> scene_pandav;
    std::shared_ptr<std::vector<YellowBot>> scene_kaurav;
    std::shared_ptr<Ball> scene_ball;
    std::shared_ptr<std::vector<Mantri>> scene_mantri;
    QVector<QGraphicsLineItem*> lines;
    float color_value;

    int frame;

    SSL_WrapperPacket state;
    SSL_GeometryData field_geometry;
    // google::protobuf::RepeatedPtrField<SSL_DetectionRobot> pandav;
    // google::protobuf::RepeatedPtrField<SSL_DetectionRobot> kaurav;

    bool* see_hotmap_;

    bool has_state_;
    // use static bool!
    bool ball_init_ = false;
    bool bots_init_ = false;

    // Ball scene_ball;
};

#endif // KSHETRA_H
