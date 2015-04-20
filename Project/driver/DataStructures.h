#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "Enums.h"

struct Order {
	button_type_t direction;
	int floor;
	int assignedIP; // Should be string? Or?
	int timeAssigned;
	bool operator==(const Order& rhs) const
	{
	    return (floor == rhs.floor && direction == rhs.direction && assignedIP == rhs.assignedIP);
	}
	bool operator < (const Order& rhs) const
	{
		if (floor < rhs.floor) 			return true;
		if (floor > rhs.floor) 			return false;
		if (direction < rhs.direction) 	return true;
		return false;
	}
};

#endif
