#ifndef Voronoi_h
#define Voronoi_h

#include <vector>
#include <list>
#include <queue>
#include <set>

#include "VPoint.h"
#include "VEdge.h"
#include "VParabola.h"
#include "VEvent.h"
#include <functional>


namespace vor
{
	/*
		Useful data containers for Vertices (places) and Edges of Voronoi diagram
	*/

	typedef std::vector<VPoint *>		Vertices	;
	typedef std::vector<VEdge *>		Edges		;

	/*
        Class for generating the Voronoi diagram using Fortune's algorithm
	*/

	class Voronoi
	{
	public:
		int id;
		Voronoi();

        /**
     * @brief Generates the Voronoi diagram.
     *
     * @param v A pointer to a list of vertices (sites) for generating the diagram.
     * @param w The width of the resulting diagram.
     * @param h The height of the resulting diagram.
     * @return A pointer to a list of edges forming the Voronoi diagram.
     */

		Edges *			GetEdges(Vertices * v, int w, int h);
		Edges *			GetEdges(Vertices * v, int w, int h, int ignore);

	private:

        Vertices *		places; ///< container of places with which we work
        Edges *			edges; ///< container of edges which will be the result
        double			width, height; ///<width and height of the diagram
        VParabola *		root; ///<the root of the tree, that represents a beachline sequence
        double			ly; ///<current "y" position of the line (see Fortune's algorithm)

        std::set<VEvent *>	deleted; ///<set  of deleted (false) Events (since we can not delete from PriorityQueue)
        std::vector<VPoint *> points; ///<list of all new points that were created during the algorithm
        std::priority_queue<VEvent *, std::vector<VEvent *>, VEvent::CompareEvent> queue; ///<priority queue with events to process

        void		InsertParabola(VPoint * p); ///<processing the place event
        void		RemoveParabola(VEvent * e); ///<processing the circle event
        void		FinishEdge(VParabola * n); ///<recursively finishes all infinite edges in the tree
        double		GetXOfEdge(VParabola * par, double y); ///<returns the current x position of an intersection point of left and right parabolas
        VParabola * GetParabolaByX(double xx); ///<returns the Parabola that is under this "x" position in the current beachline
		double		GetY(VPoint * p, double x);
        void		CheckCircle(VParabola * b); ///<checks the circle event (disappearing) of this parabola
		VPoint *	GetEdgeIntersection(VEdge * a, VEdge * b);
	};
}

#endif
