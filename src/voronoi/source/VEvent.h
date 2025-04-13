#ifndef VEvent_h
#define VEvent_h

#include <iostream>
#include "VPoint.h"
#include "VParabola.h"

/**
 * @class VEvent
 * @brief Class for storing a site or circle event in the Fortune's algorithm event queue.
 *
 * @var VPoint* VEvent::point
 * @brief The point at which the event occurs.
 *
 * @var bool VEvent::pe
 * @brief Whether the event is a site event.
 *
 * @var double VEvent::y
 * @brief The y-coordinate of the event's point. Used for event queue sorting.
 *
 * @var VParabola* VEvent::arch
 * @brief if "pe", it is an arch above which the event occurs
 */

class VEvent
{
public:
	VPoint *	point;
	bool		pe;
	double		y;
	VParabola * arch;

    /**
     * @brief Constructs a VEvent object.
     *
     * @param p The point at which the event occurs.
     * @param pev Whether the event is a place (site) event.
     */
	
	VEvent(VPoint * p, bool pev)
	{
		point	= p;
		pe		= pev;
		y		= p->y;
		arch	= 0;
	}
   /**
     * @struct CompareEvent
     * @brief Comparator for VEvent pointers, used in priority queues.
     *
     * Compares based on the y-coordinate of the event (higher priority for smaller y).
     */
	struct CompareEvent : public std::binary_function<VEvent*, VEvent*, bool>
	{
		bool operator()(const VEvent* l, const VEvent* r) const { return (l->y < r->y); }
	};
};

#endif
