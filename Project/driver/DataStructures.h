#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "Enums.h"
#include <string>

struct Order {
	button_type_t direction;
	int floor;
	std::string assignedIP;
	int timeAssigned;

	Order() {
		direction = BUTTON_COMMAND;
		floor = -1;
		assignedIP = "";
		timeAssigned = -1;
	}
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

	operator bool() const
	{
		if (floor == -1) return false;
		return true;
	}
};

struct Offer {
	int cost;
	int floor;
	button_type_t direction;
	std::string IP;
};

#endif