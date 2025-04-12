#ifndef VParabola_h
#define VParabola_h

#include "VPoint.h"
#include "VEdge.h"

/**
 * @brief Parabolas and events have pointers to each other, so we declare class VEvent, which will be defined later.
 */

class VEvent;

/*
	A class that stores information about an item in beachline sequence (see Fortune's algorithm). 
	It can represent an arch of parabola or an intersection between two archs (which defines an edge).
	In my implementation I build a binary tree with them (internal nodes are edges, leaves are archs).
*/

class VParabola
{
public:

	/*
		isLeaf		: flag whether the node is Leaf or internal node
		site		: pointer to the focus point of parabola (when it is parabola)
		edge		: pointer to the edge (when it is an edge)
		cEvent		: pointer to the event, when the arch disappears (circle event)
		parent		: pointer to the parent node in tree
	*/

	bool		isLeaf;
	VPoint *	site;
	VEdge *		edge;
	VEvent *	cEvent;
	VParabola * parent;

    /**
     * @brief Default constructor of the class (empty for edge, with focus parameter for an arch).
     */

	VParabola	();
	VParabola	(VPoint * s);

	/*
		Access to the children (in tree).
	*/

	void		SetLeft (VParabola * p) {_left  = p; p->parent = this;}
	void		SetRight(VParabola * p) {_right = p; p->parent = this;}

	VParabola *	Left () { return _left;  }
	VParabola * Right() { return _right; }

    /**
     * @brief Some useful tree operations:
     *
     * GetLeft           : Returns the closest left leaf of the tree.
     * GetRight          : Returns the closest right leaf of the tree.
     * GetLeftParent     : Returns the closest parent which is on the left.
     * GetRightParent    : Returns the closest parent which is on the right.
     * GetLeftChild      : Returns the closest leaf which is on the left of the current node.
     * GetRightChild     : Returns the closest leaf which is on the right of the current node.
     */

	static VParabola * GetLeft			(VParabola * p);
	static VParabola * GetRight			(VParabola * p);
	static VParabola * GetLeftParent	(VParabola * p);
	static VParabola * GetRightParent	(VParabola * p);
	static VParabola * GetLeftChild		(VParabola * p);
	static VParabola * GetRightChild	(VParabola * p);
	
private:

	VParabola * _left;
	VParabola * _right;
};

#endif
