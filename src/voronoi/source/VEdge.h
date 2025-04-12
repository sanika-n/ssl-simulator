#ifndef VEdge_h
#define VEdge_h

#include "VPoint.h"
#include <iostream>

/**
 * @class VEdge
 * @brief A class that stores an edge in the Voronoi diagram.
 */

class VEdge
{
public:

    //passing address much easier than passing value
    VPoint *	start;  ///pointer to start point
    VPoint *	end; ///pointer to end point
    VPoint *	direction; /// Directional vector from start to end; normal of the line formed by |left, right|.
    VPoint *	left; ///pointer to Voronoi place on the left side of edge
    VPoint *	right; ///pointer to Voronoi place on the right side of edge
	
    double		f;	/// Directional slope of the edge (used in y = f*x + g).
    double		g; 	/// Y-intercept of the edge (used in y = f*x + g).

    VEdge * neighbour; /// In case the edge has two parts, this pointer connects to the other part at the end of an algorithm.

    /**
     * @brief Constructor of the VEdge class.
     * @param s Pointer to the start point of the edge.
     * @param a Pointer to the left Voronoi site.
     * @param b Pointer to the right Voronoi site.
     */
	VEdge(VPoint * s, VPoint * a, VPoint * b)
	{
		start		= s;
		left		= a;
		right		= b;
		neighbour	= 0;
		end			= 0;

		f = (b->x - a->x) / (a->y - b->y) ;
		g = s->y - f * s->x ;
		direction = new VPoint(b->y - a->y, -(b->x - a->x));
	}

    /**
     * @brief Destructor of the VEdge class.
     *
     * Deletes the direction vector, which is owned exclusively by this edge.
     * Other pointers may be shared across multiple edges and are not deleted here.
     */
	~VEdge()
	{
		delete direction ;
	}
	
};

#endif
