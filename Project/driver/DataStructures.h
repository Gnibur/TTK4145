#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "Enums.h"
#include <vector>
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

	Order(button_type_t d, int f, std::string i, int t)
	{
		direction = d;
		floor = f;
		assignedIP = i;
		timeAssigned = t;
	}	

	bool operator==(const Order& rhs) const
	{
	    if ((rhs.floor == floor) && (rhs.direction == direction) &&  (assignedIP.compare(rhs.assignedIP) == 0))
			return true;
		else if ((rhs.floor == floor) && (rhs.direction == direction) && (direction != BUTTON_COMMAND))
			return true;
		else
			return false;
	}
	bool operator < (const Order& rhs) const
	{
		if (floor < rhs.floor) 					return true;
		if (floor > rhs.floor) 					return false;
		if (direction < rhs.direction) 			return true;
		if (direction > rhs.direction)			return false;
		if (assignedIP > rhs.assignedIP)		return true;
		return false;
	}

	operator bool() const
	{
		if (floor == -1) return false;
		return true;
	}
};

typedef std::vector<Order> OrderList;

struct Offer {
	int cost;
	int floor;
	button_type_t direction;
	std::string fromIP;

	Offer() {
		cost = -1;
		floor = -1;
		direction = BUTTON_COMMAND;
		fromIP = "invalidIP";
	}

	Offer(int c, int f, button_type_t d, std::string i)
	{
		cost = c;
		floor = f;
		direction = d;
		fromIP = i;
	}

	bool operator < (const Offer& rhs) const
	{
		if (cost < rhs.cost) 					return true;
		if (cost > rhs.cost) 					return false;
		return false;
	}
};

#endif
