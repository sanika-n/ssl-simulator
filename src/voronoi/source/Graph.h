#ifndef graph_h
#define graph_h

#include "Voronoi.h"
#include <vector>
#include <map>

using namespace std;

namespace gra{

    /**
     * @brief Class representing a graph built from Voronoi edges.
     *
     * The class provides methods for displaying vertices and edges, finding the nearest vertex,
     * calculating shortest paths, and retrieving specific vertices by their IDs.
     */
    class Graph{
        public:
            Graph(vor::Edges * e);

            void display();

            int getNearestVertex(double x, double y);

            pair<int, int> getPathEndpoints(pair<int, int> startVertices, pair<int, int> endVertices, double startx, double starty, double endx, double endy);

        /**
         * @brief Computes the Euclidean distance between two points.
         * @param A The first point (x, y).
         * @param B The second point (x, y).
         * @return The Euclidean distance between the two points.
         */
            double dist(pair<double, double>A, pair<double, double> B);

        /**
         * @brief Computes the shortest path between two vertices using an algorithm like Dijkstra's.
         * @param start The starting vertex ID.
         * @param end The destination vertex ID.
         * @return A vector containing the vertex IDs along the shortest path.
         */
            vector<int> shortestPath(int start, int end);

            pair<double, double> getVertexById(int id);
            vector<int> vertices;
            vector<pair<double,pair<int, int>>> edges;
            map<int, VPoint*> idmap;
    };
}


#endif
