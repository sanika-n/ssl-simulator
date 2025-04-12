/**
 * @file kshetra.h
 * @brief Defines the Kshetra simulator, the ground where an epic battle unfolds.
 * 
 * This file includes the necessary components for the simulation, such as Yodha,
 * Mantri, Drona, and various helper functions to manage the graphics scene.
 * 
 * @note If you're familiar with mythology, you'll recognize the names of the components.
 */


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

//primitive graphics function for later use
// convenience functions that draw some basic stuff

/**
 * @note The Following 3 functions are helper functions to draw circles, lines and arcs respecitvely.
 */


/**
 * @brief a helper function to draw a circle in the scene
 * 
 * @param scene Pointer to the QGraphicsScene where the arc will be drawn.
 * @param center The center of the circle as a QPointF object
 * @param radius Radius of the circle
 * @param color Color of the circle as a QColor object
 * @return QGraphicsEllipseItem* A pointer to the created circle
 */
inline QGraphicsEllipseItem* addCircle_(QGraphicsScene *scene, const QPointF &center, int radius, QColor color);


/**
 * @brief a helper function to draw a line in the scene
 * 
 * @param scene Pointer to the QGraphicsScene where the arc will be drawn.
 * @param point1 first point of the line as a QPointF object
 * @param point2 second point of the line as a QpointF object
 * @param width width of the line as in integer
 * @return QGraphicsLineItem* A pointer to the created line
 */
inline QGraphicsLineItem* addLine_(QGraphicsScene *scene, const QPointF &point1, const QPointF &point2, int width);


/**
 * @brief a helper function to draw a arc in the scene
 * 
 * @param scene Pointer to the QGraphicsScene where the arc will be drawn.
 * @param center The center of the arcs's circle as a QPointF object
 * @param radius Radius of the circle correspoding to the arc
 * @param width Width of the arc
 * @return QGraphicsEllipseItem* A pointer to the created circle
 */
inline QGraphicsEllipseItem* addArc_(QGraphicsScene *scene, const QPointF center, int radius, int width);


/**
 * @class Ksetra
 * @brief The Kshetra class represents the battlefield where the simulation takes place, ie the football
 * 
 * It draws the field, places the ball and the players on the field accroding to the messages received trhough ProtoBuff
 * It can optionally show the heatmap, that also gets updated every 10 frames.
 */
class Kshetra : public QGraphicsView
{
    Q_OBJECT
public:

    explicit Kshetra( QWidget *parent=0);
    /**
    * @brief a function to that converts mm-based coordinates (protobuf format) to cm-based QGraphicsScene coordinates. Also shifts the coordinate system by 
    *setting the origin to the center of the scene.
    * @param point QPointF object to be tranformed
    * @return QPointF object that has been transformed
    */    
    inline QPointF transformToScene(QPointF &&point);

    /**
    * @brief A function that shares the ownership of the pandav and kaurav vectors to the Kshetra class
    * 
    * @param pandav A vector of BlueBot objects representing the pandav players
    * @param kaurav A vector of YellowBot objects representing the kaurav players
    */
    void setPlayers(std::shared_ptr<std::vector<BlueBot>> pandav,std::shared_ptr<std::vector<YellowBot>> kaurav);

    /**
    * @brief A function that shares the ownership of the Ball class with the Kshetra class
    * @param ball A shared pointer to a Ball object representing the ball in the simulation
    */
    void setBall(std::shared_ptr<Ball> ball);

    /**
    * @brief sets up the scene and the scene_hotmap to a normal foorball field
    * 
    * @param length takes the length in mm
    * @param width takes the width of the filed in mm
    */
    void setGround(qint32 length, qint32 width);

    /**
    * @brief draws all the lines and arcs on to the scene
    * @param field_info The path of a protobuf object that contains the field lines and arcs
    */
    void setFieldLines(const SSL_GeometryFieldSize &field_info);

    /**
    * @brief a helper function to draw a arc in the scene
    * 
    * @param scene Pointer to the QGraphicsScene where the arc will be drawn.
    * @param center The center of the arcs's circle as a QPointF object
    * @param radius Radius of the circle correspoding to the arc
    * @param width Width of the arc
    * @return QGraphicsEllipseItem* A pointer to the created circle
    */

     /**
    * @brief a toggle like function used to set the hotMap on and off.
    */
    void viewHotMap();
    const QGraphicsScene *getScene(){
        if(!see_hotmap_) return scene;
        else return scene_hotmap;
    }
    ~Kshetra();

public slots:
    /**
    * @brief Draws field and ball as received from vyasa in protobuff format, initializes the field only once and then if sethotmap is true, draws the hotmap
    *
    * @param buffer
    *
    * Called when Vyasa::receivedState signal is emitted
    *
    * Initializes ball if it doesn't exist, or updates position
    *
    * Sets scene if not set
    */
    void handleState(QByteArray *buffer);

    /**
    * @brief This function deletes any existing linesa and draws lines connecting aadjacent vertices in the vector vertices.
    * 
    * @param vertices A vector of QPointF objects representing the vertices of the graph
    * @return QGraphicsEllipseItem* A pointer to the created circle
    */
    void handleGraph(std::vector<QPointF> *vertices);

    /**
    * @brief Initializes the hotmap if not done already and updates it every 10 frames
    */
    void setHotMap();

private:
    QGraphicsScene *scene;
    QGraphicsScene *scene_hotmap;
    QGraphicsScene *scene_hotmap_future;
    QPainter *painter;
    std::shared_ptr<std::vector<BlueBot>> scene_pandav;
    std::shared_ptr<std::vector<YellowBot>> scene_kaurav;
    std::shared_ptr<Ball> scene_ball;
    std::shared_ptr<std::vector<Mantri>> scene_mantri;
    QVector<QGraphicsLineItem*> lines;
    float color_value;

    int frame;

    SSL_WrapperPacket state;
    SSL_GeometryData field_geometry;
    bool see_hotmap_;

    bool has_state_;
    bool ball_init_ = false;
    bool bots_init_ = false;
};

#endif // KSHETRA_H
