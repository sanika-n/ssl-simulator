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
 * @brief Clips Voronoi edges to a bounding box of width w and height h.
 *
 * This function removes any edges that lie completely outside the field bounds.
 * It doesn't clip/intersect edges that partially go out — it just skips them for now.
 *
 * @param original The original list of Voronoi edges.
 * @param w Width of the bounding box.
 * @param h Height of the bounding box.
 * @return Clipped list of edges inside the bounding box.
 */
vor::Edges* clipToBoundingBox(vor::Edges* original, double w, double h) {
    auto* clipped = new vor::Edges();

    for (auto* edge : *original) {
        if (!edge->start || !edge->end)
            continue;

        auto x1 = edge->start->x, y1 = edge->start->y;
        auto x2 = edge->end->x, y2 = edge->end->y;

        // Skip edges that go completely outside the bounding box
        if ((x1 < 0 && x2 < 0) || (x1 > w && x2 > w) ||
            (y1 < 0 && y2 < 0) || (y1 > h && y2 > h))
            continue;

        clipped->push_back(edge);
    }

    return clipped;
}


/**
 * @brief Plans a path using Voronoi diagram and graph-based shortest path.
 *
 * This function receives positions of all bots, creates Voronoi diagrams using them as obstacles,
 * adds buffer points around the start and end points, constructs a graph from the Voronoi edges,
 * and calculates the shortest path from start to end.
 *
 * @param my_team Vector of positions of all bots
 * @param endpt Target endpoint coordinates
 * @param target_id ID of the bot which is to move (used to find start point)
 * @return Vector of waypoints (QPointF) representing the planned path
 */
vector<QPointF> plan_path(vector<pair<double, double>> my_team,vector<pair<double, double>> enemy_team, pair<double, double> endpt, int target_id)
{
    LOG << "started";
    using namespace vor;

    v = new Voronoi();
    ver = new Vertices();   // ver stores the positions of all adversary bots

    int id=0;

    //IS THIS NEEDED????
    // Add friendly bots as obstacles
    for(int i=0; i < my_team.size(); ++i)
    {   //if(i==target_id) continue;
        ver->push_back(new VPoint( my_team[i].first , my_team[i].second, id++));
        }

    // Add all enemy bots as obstacles
    for (const auto& bot : enemy_team)
    {
        ver->push_back(new VPoint(bot.first, bot.second, id++));
    }

    // Add buffer points for start and end
    int val =200;
    // start point front
    const auto& start_bot = my_team[target_id];
    ver->push_back(new VPoint( start_bot.first + val*0.766, start_bot.second + val*0.642, id++));
    ver->push_back(new VPoint( start_bot.first + val*0.342, start_bot.second - val*0.939, id++));
    ver->push_back(new VPoint( start_bot.first - val*0.984, start_bot.second + val*0.173, id++));

    // end point front
    ver->push_back(new VPoint( endpt.first + val*0.766, endpt.second + val*0.642,id++));
    ver->push_back(new VPoint( endpt.first + val*0.342, endpt.second - val*0.939, id++));
    ver->push_back(new VPoint( endpt.first - val*0.984, endpt.second + val*0.173, id++));

    // LOG << 1;
    // make w and h large if cup happens
    edg = v->GetEdges(ver, w, h, target_id);
    edg = clipToBoundingBox(edg, w, h);

    // GetEdges takes the vertices and the bounding box size as input
    // and generates voronoi diagram, returning all the edges.

    // LOG << 2;
    // Construct graph using Voronoi edges
    Graph g = Graph(edg);

    // Get nearest Voronoi vertex to bot (start) and endpoint
    int start_id = g.getNearestVertex(start_bot.first, start_bot.second);
    int end_id = g.getNearestVertex(endpt.first, endpt.second);

    // Get shortest path from start to end in graph
    vector<int> path = g.shortestPath(start_id, end_id);
    // LOG << 3;

    //i goes till path.size()-1 so that bot can start by going to the next point on the path
    // Convert path vertices into QPointF for visualization or control
    vector<QPointF> pos;
    for(int i=0;i<path.size()-1;i++){
        auto p = g.getVertexById(path[i]);
        pos.push_back(QPointF(p.first, p.second));
    }
    LOG << "ended" << path.size();
    return pos;
}
