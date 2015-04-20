#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "Enums.h"
#include <string>

struct Order {
	button_type_t direction;
	int floor;
	std::string assignedIP;
	int timeAssigned;
	bool operator==(const Order& rhs) const
	{
	    return (floor == rhs.floor && direction == rhs.direction && assignedIP.compare(rhs.assignedIP) == 0);
	}
	bool operator < (const Order& rhs) const
	{
		if (floor < rhs.floor) 					return true;
		if (floor > rhs.floor) 					return false;
		if (direction < rhs.direction) 			return true;
		if (direction > rhs.direction)			return false;
		if (timeAssigned > rhs.timeAssigned)	return true;
		return false;
	}
};

#endif
