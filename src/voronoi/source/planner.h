#ifndef Planner_H
#define Planner_H
#include <GL/glew.h> // Include the GLEW header file
#include <vector>
#include <GL/glut.h> // Include the GLUT header file
#include <iostream>
#include <math.h>
#include "Graph.h"
#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QDebug>
#include "Voronoi.h"
#include "VPoint.h"

/**
 * @brief Plans a path for a bot using Voronoi-based path planning.
 */

vor::Edges* clipToBoundingBox(vor::Edges* original, double w, double h);
std::vector<QPointF> plan_path(std::vector<std::pair<double, double>> my_team, std::vector<std::pair<double, double>> enemy_team, std::pair<double, double> endpt, int target_id);

#endif //Planner
