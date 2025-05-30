
#include "Voronoi.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <cmath>

using namespace vor;

Voronoi::Voronoi()
{
	edges = 0;
	id = 0;
}

Edges * Voronoi::GetEdges(Vertices * v, int w, int h)
{
	places = v; //places are sites
	width = w;
	height = h;
	root = 0;

	if(!edges) edges = new Edges();
	else 
	{
		for(Vertices::iterator	i = points.begin(); i != points.end(); ++i) delete (*i);
		for(Edges::iterator		i = edges->begin(); i != edges->end(); ++i) delete (*i);
		points.clear();
		edges->clear();
	}
	
	for(Vertices::iterator i = places->begin(); i!=places->end(); ++i)
	{
		queue.push(new VEvent( *i, true));
	}

	VEvent * e;
	while(!queue.empty())
	{
		e = queue.top();
		queue.pop();
		ly = e->point->y;
		if(deleted.find(e) != deleted.end()) { delete(e); deleted.erase(e); continue;}
		if(e->pe) InsertParabola(e->point);
		else RemoveParabola(e);
		delete(e);
	}
	
	FinishEdge(root);

	for(Edges::iterator i = edges->begin(); i != edges->end(); ++i)
	{
		if( (*i)->neighbour) 
		{
			(*i)->start = (*i)->neighbour->end;
			delete (*i)->neighbour;
		}
	}
	
	return edges;
}


/**
 * @brief Constructs the Voronoi diagram and returns the list of edges.
 *
 * This is the main function that performs Fortune's algorithm. It processes a set of input sites,
 * handles events (site and circle), builds the beachline, finalizes any unfinished edges, and
 * returns the computed Voronoi edges.
 *
 * @param v Pointer to a list of input site points (vertices).
 * @param w Width of the bounding box for the diagram.
 * @param h Height of the bounding box for the diagram.
 * @param ignore ID of the site to be excluded from the diagram (if any).
 * @return Pointer to the list of Voronoi edges generated.
 */
Edges * Voronoi::GetEdges(Vertices * v, int w, int h, int ignore)
{
    places = v; ///<places are input sites
	width = w;
	height = h;
    root = 0; ///<initialise beachline root

	if(!edges) edges = new Edges();
	else 
	{
        for(Vertices::iterator	i = points.begin(); i != points.end(); ++i) delete (*i);
		for(Edges::iterator		i = edges->begin(); i != edges->end(); ++i) delete (*i);
		points.clear();
		edges->clear();
	}

    // Insert site events into the priority queue, ignoring the excluded site
    for(Vertices::iterator i = places->begin(); i!=places->end(); ++i)
	{
		if((*i)->id != ignore)
		queue.push(new VEvent( *i, true));
	}

	VEvent * e;
	while(!queue.empty())
	{
		e = queue.top();
		queue.pop();
        ly = e->point->y; ///< Update the current sweepline position (ly = last y).

        if(deleted.find(e) != deleted.end()) { delete(e); deleted.erase(e); continue;} //lazy deletion: skip deleted events

        // Process the event: insert new parabola or handle circle removal
		if(e->pe) InsertParabola(e->point);
		else RemoveParabola(e);
		delete(e);
	}
	
	FinishEdge(root);

    // Resolve unfinished edge segments by stitching with their neighbor segments
	for(Edges::iterator i = edges->begin(); i != edges->end(); ++i)
	{
		if( (*i)->neighbour) 
		{
			(*i)->start = (*i)->neighbour->end;
			delete (*i)->neighbour;
		}
	}
	
	return edges;
}


/**
 * @brief Handles a site event by inserting a new parabola (arc) into the beachline.
 *
 * This function updates the beachline structure (represented as a binary tree of parabolas)
 * when a new site is encountered. It may:
 * - Create a new root if this is the first site.
 * - Handle a degenerate case when two sites are horizontally aligned.
 * - Split an existing arc and insert the new arc in the correct place.
 * - Add new edges to the Voronoi diagram.
 * - Cancel invalid circle events and trigger new ones if needed.
 *
 * @param p Pointer to the new site (focus of the new parabola).
 */
void	Voronoi::InsertParabola(VPoint * p)
{
    // First site – create root
	if(!root){root = new VParabola(p); return;}

    // Degenerate case: two sites with (almost) same y-coordinate
    if(root->isLeaf && root->site->y - p->y < 1)
	{
		VPoint * fp = root->site;
		root->isLeaf = false;
        root->SetLeft(new VParabola(fp));
        root->SetRight(new VParabola(p));

        // Create starting point of edge (midpoint between both sites)
		VPoint * s = new VPoint((p->x + fp->x)/2, height, id++); // za��tek hrany uprost�ed m�st
		points.push_back(s);

		if(p->x > fp->x) root->edge = new VEdge(s, fp, p); // rozhodnu, kter� vlevo, kter� vpravo
		else root->edge = new VEdge(s, p, fp);
		edges->push_back(root->edge);
		return;
	}

    VParabola * par = GetParabolaByX(p->x); //arc above new site

    // Cancel any pending circle event for the found arc
	if(par->cEvent)
	{
		deleted.insert(par->cEvent);
		par->cEvent = 0;
	}

    // Create a new edge starting at the intersection
	VPoint * start = new VPoint(p->x, GetY(par->site, p->x), id++);
	points.push_back(start);

	VEdge * el = new VEdge(start, par->site, p);
	VEdge * er = new VEdge(start, p, par->site);

	el->neighbour = er;
	edges->push_back(el);

    // Split the arc into three new ones and insert new arc in the middle
	par->edge = er;
	par->isLeaf = false;

	VParabola * p0 = new VParabola(par->site);
	VParabola * p1 = new VParabola(p);
	VParabola * p2 = new VParabola(par->site);

	par->SetRight(p2);
	par->SetLeft(new VParabola());
	par->Left()->edge = el;

	par->Left()->SetLeft(p0);
	par->Left()->SetRight(p1);

    // Check for potential circle events caused by the new configuration
	CheckCircle(p0);
	CheckCircle(p2);
}

/**
 * @brief Handles a circle event by removing an arc (parabola) from the beachline.
 *
 * This function is part of Fortune's algorithm. When three sites form a circle, the middle arc
 * disappears and a Voronoi vertex is formed. This function updates the structure by:
 * - Creating the Voronoi vertex
 * - Connecting edges to this vertex
 * - Removing the arc from the beachline tree
 * - Rechecking for new circle events caused by the change
 *
 * @param e Pointer to the VEvent representing the circle event.
 */
void	Voronoi::RemoveParabola(VEvent * e)
{
	VParabola * p1 = e->arch;

	VParabola * xl = VParabola::GetLeftParent(p1);
	VParabola * xr = VParabola::GetRightParent(p1);

	VParabola * p0 = VParabola::GetLeftChild(xl);
	VParabola * p2 = VParabola::GetRightChild(xr);

    //sanity check
    if(p0 == p2) std::cout << "error - right and left parabola have the same focus!\n";

    // Cancel any pending circle events for p0 and p2
	if(p0->cEvent){ deleted.insert(p0->cEvent); p0->cEvent = 0; }
	if(p2->cEvent){ deleted.insert(p2->cEvent); p2->cEvent = 0; }

    // Compute Voronoi vertex where three arcs meet
	VPoint * p = new VPoint(e->point->x, GetY(p1->site, e->point->x), id++);
	points.push_back(p);

	xl->edge->end = p;
	xr->edge->end = p;

    // Find which of xl or xr is higher in the tree to attach a new edge there.
	VParabola * higher;
	VParabola * par = p1;
	while(par != root)
	{
		par = par->parent;
		if(par == xl) higher = xl;
		if(par == xr) higher = xr;
	}
	higher->edge = new VEdge(p, p0->site, p2->site);
	edges->push_back(higher->edge);

    // Reconnect the tree, bypassing the removed arc p1 and its parent.
	VParabola * gparent = p1->parent->parent;
	if(p1->parent->Left() == p1)
	{
		if(gparent->Left()  == p1->parent) gparent->SetLeft ( p1->parent->Right() );
		if(gparent->Right() == p1->parent) gparent->SetRight( p1->parent->Right() );
	}
	else
	{
		if(gparent->Left()  == p1->parent) gparent->SetLeft ( p1->parent->Left()  );
		if(gparent->Right() == p1->parent) gparent->SetRight( p1->parent->Left()  );
	}

	delete p1->parent;
	delete p1;

    // Check if new circle events occur due to tree updat
	CheckCircle(p0);
	CheckCircle(p2);
}

/**
 * @brief Completes unfinished edges in the beachline tree after the sweep line is done.
 *
 * This recursive function traverses the beachline binary tree and finalizes
 * edges that were still growing (i.e., had only a starting point) by assigning them
 * an artificial "far-away" endpoint based on their direction.
 *
 * @param n A pointer to the current node in the beachline tree.
 */
void	Voronoi::FinishEdge(VParabola * n)
{
	if(n->isLeaf) {delete n; return;}
	double mx;

    // Choose a far x-coordinate depending on the direction of the edge
    if(n->edge->direction->x > 0.0)
        mx = std::max(width, n->edge->start->x + 10 );
    else
        mx = std::min(0.0, n->edge->start->x - 10);

    // Calculate the y-coordinate using the line equation
	VPoint * end = new VPoint(mx, mx * n->edge->f + n->edge->g, id++); 
	n->edge->end = end;
	points.push_back(end);

    // Recursively finish left and right subtrees
	FinishEdge(n->Left() );
	FinishEdge(n->Right());
	delete n;
}

double	Voronoi::GetXOfEdge(VParabola * par, double y)
{
	VParabola * left = VParabola::GetLeftChild(par);
	VParabola * right= VParabola::GetRightChild(par);

	VPoint * p = left->site;
	VPoint * r = right->site;

	double dp = 2.0 * (p->y - y);
	double a1 = 1.0 / dp;
	double b1 = -2.0 * p->x / dp;
	double c1 = y + dp / 4 + p->x * p->x / dp;
			
		   dp = 2.0 * (r->y - y);
	double a2 = 1.0 / dp;
	double b2 = -2.0 * r->x/dp;
	double c2 = ly + dp / 4 + r->x * r->x / dp;
			
	double a = a1 - a2;
	double b = b1 - b2;
	double c = c1 - c2;
			
	double disc = b*b - 4 * a * c;
	double x1 = (-b + std::sqrt(disc)) / (2*a);
	double x2 = (-b - std::sqrt(disc)) / (2*a);

	double ry;
	if(p->y < r->y ) ry =  std::max(x1, x2);
	else ry = std::min(x1, x2);

	return ry;
}

VParabola * Voronoi::GetParabolaByX(double xx)
{
	VParabola * par = root;
	double x = 0.0;

    while(!par->isLeaf) // traverse the tree until a suitable leaf is found
	{
		x = GetXOfEdge(par, ly);
		if(x>xx) par = par->Left();
		else par = par->Right();				
	}
	return par;
}

double	Voronoi::GetY(VPoint * p, double x) // focus, x-coordinate
{
	double dp = 2 * (p->y - ly);
	double a1 = 1 / dp;
	double b1 = -2 * p->x / dp;
	double c1 = ly + dp / 4 + p->x * p->x / dp;
	
	return(a1*x*x + b1*x + c1);
}

void	Voronoi::CheckCircle(VParabola * b)
{
	VParabola * lp = VParabola::GetLeftParent (b);
	VParabola * rp = VParabola::GetRightParent(b);

	VParabola * a  = VParabola::GetLeftChild (lp);
	VParabola * c  = VParabola::GetRightChild(rp);

	if(!a || !c || a->site == c->site) return;

	VPoint * s = 0;
	s = GetEdgeIntersection(lp->edge, rp->edge);
	if(s == 0) return;

	double dx = a->site->x - s->x;
	double dy = a->site->y - s->y;

	double d = std::sqrt( (dx * dx) + (dy * dy) );

	if(s->y - d >= ly) { return;}

	VEvent * e = new VEvent(new VPoint(s->x, s->y - d, id++), false);
	points.push_back(e->point);
	b->cEvent = e;
	e->arch = b;
	queue.push(e);
}

VPoint * Voronoi::GetEdgeIntersection(VEdge * a, VEdge * b)
{
	double x = (b->g-a->g) / (a->f - b->f);
	double y = a->f * x + a->g;

	if((x - a->start->x)/a->direction->x < 0) return 0;
	if((y - a->start->y)/a->direction->y < 0) return 0;
		
	if((x - b->start->x)/b->direction->x < 0) return 0;
	if((y - b->start->y)/b->direction->y < 0) return 0;	

	VPoint * p = new VPoint(x, y, id++);		
	points.push_back(p);
	return p;
}


	
