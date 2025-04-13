#ifndef VISUALIZE_H
#define VISUALIZE_H

#include "QGraphicsItem"
#include "QGraphicsScene"

/*!
 * \class Visualize
 * \brief Implements basic visualization features for the simulator.
 *
 * This class manages drawing and clearing per frame.
 * Users can add frame-based objects and update tracked objects.
 *
 * - FrameObjects are temporary and recreated each frame.
 * - TrackedObjects persist but must be manually updated.
 *
 * Tip: To follow a TrackedObject, attach another QGraphicsItem to it.
 */
class Visualize
{
    // this class implements
    // basic visualization features
    // for the simulator.
    // manage the drawing and clearing at each frame
    // within this class and only allow user to add
    // Objects and update them.
    // TrackedObjects have a longer lifetime than FrameObjects.
    // they will persist, but one has to update them. How does
    // the user keep a handle to these? a nice idea, attach
    // another graphicsitem to a TrackedObject, it will follow this
    // object around.
    // for now, focussing on getting frameobjects done.

public:
    //! Default pen color used for drawing (red, width = 1).
    static const QPen default_color;

    /*!
     * \brief Constructor with scene injection.
     * \param scene Pointer to QGraphicsScene where items will be drawn.
     */
    Visualize(QGraphicsScene *scene): scene(scene){}

    /*!
     * \brief Adds a line between two points to the scene.
     * \param pos1 Starting point of the line.
     * \param pos2 Ending point of the line.
     * \param color Pen to use for drawing the line. Defaults to red.
     */
    void add_line(QPointF &pos1, QPointF &pos2, QPen color=default_color){
        m_lines.push_back(scene->addLine(QLineF(pos1, pos2), color));
    }

    /*!
     * \brief Draws interpolated lines through a sequence of 2D points.
     * \param points A vector of (x, y) coordinate pairs.
     */
    void interpolate(std::vector<std::pair<double, double>> &points);

    /*!
     * \brief Deletes a specific QGraphicsItem from the scene.
     * \param item The item to be removed.
     */
    void delete_object(QGraphicsItem *item){
        scene->removeItem(item);
    }

private:
    //! Private constructor to prevent default instantiation.
    Visualize(); //making this private, so that no one can call it

    //! Stores all line items added in the current frame.
    QVector<QGraphicsLineItem *> m_lines;

    //! The graphics scene to which this visualizer is attached.
    QGraphicsScene *scene;
};

//! Static member initialization for default drawing color.
const QPen Visualize::default_color  = QPen(Qt::red, 1);

#endif // VISUALIZE_H
