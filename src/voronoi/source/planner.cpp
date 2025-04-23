#include "planner.h"

vor::Voronoi * v;
vor::Vertices * ver; // vertices
vor::Vertices * dir; // velocities of vertices
vor::Edges * edg;	 // edges of the Voronoi diagram

double w = 900; //width of bounding box
double h = 600; //height of bounding box

int target_id = 5;

pair<double, double> startpt, endpt;

/**
 * @brief Calculates the squared Euclidean distance between two points.
 *
 * @param p1 First point
 * @param p2 Second point
 * @return Squared distance between p1 and p2
 */
double dist(pair<double, double> &p1, pair<double, double> &p2){
    double x = p1.first-p2.first;
    double y = p1.second-p2.second;
    x*=x;
    y*=y;
    double d = (x+y);
    return d;
}

#define LOG qDebug() << "[planner]: "
using namespace gra;

/**
 * @brief Plans a path using Voronoi diagram and graph-based shortest path.
 *
 * This function receives positions of all bots, creates Voronoi diagrams using them as obstacles,
 * adds buffer points around the start and end points, constructs a graph from the Voronoi edges,
 * and calculates the shortest path from start to end.
 *
 * @param bot_pos Vector of positions of all bots
 * @param endpt Target endpoint coordinates
 * @param target_id ID of the bot which is to move (used to find start point)
 * @return Vector of waypoints (QPointF) representing the planned path
 */
vector<QPointF> plan_path(vector<pair<double, double>> bot_pos, pair<double, double> endpt, int target_id)
{
    using namespace vor;

    v = new Voronoi();
    ver = new Vertices();   // ver stores the positions of all adversary bots

    // Add all bot positions as obstacles
    for(int i=0; i < bot_pos.size(); ++i)
    {
        ver->push_back(new VPoint( bot_pos[i].first , bot_pos[i].second, i));
    }

    // Add buffer points for start and end
    int val = 200;
    // start point front
    ver->push_back(new VPoint( ver->at(target_id)->x + val*0.766, ver->at(target_id)->y + val*0.642, bot_pos.size()));
    ver->push_back(new VPoint( ver->at(target_id)->x + val*0.342, ver->at(target_id)->y - val*0.939, bot_pos.size() + 1));
    ver->push_back(new VPoint( ver->at(target_id)->x - val*0.984, ver->at(target_id)->y + val*0.173, bot_pos.size() + 2));

    // end point front
    ver->push_back(new VPoint( endpt.first + val*0.766, endpt.second + val*0.642, bot_pos.size() + 3));
    ver->push_back(new VPoint( endpt.first + val*0.342, endpt.second - val*0.939, bot_pos.size() + 4));
    ver->push_back(new VPoint( endpt.first - val*0.984, endpt.second + val*0.173, bot_pos.size() + 5));

    // LOG << 1;
    // make w and h large if cup happens
    edg = v->GetEdges(ver, w, h, target_id);

    // GetEdges takes the vertices and the bounding box size as input
    // and generates voronoi diagram, returning all the edges.

    // LOG << 2;
    // Construct graph using Voronoi edges
    Graph g = Graph(edg);

    // Get nearest Voronoi vertex to bot (start) and endpoint
    auto nearest_index = g.getNearestVertex(ver->at(target_id)->x, ver->at(target_id)->y);
    int start_id = nearest_index;
    int end_id = g.getNearestVertex(endpt.first, endpt.second);

    // Get shortest path from start to end in graph
    vector<int> path = g.shortestPath(start_id, end_id);
    // LOG << 3;

    // Convert path vertices into QPointF for visualization or control
    vector<QPointF> pos;
    for(int i=0;i<path.size()-1;i++){
        auto p = g.getVertexById(path[i]);
        pos.push_back(QPointF(p.first, p.second));
    }
    return pos;
}
