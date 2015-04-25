#include "DataStructures.h"
#include "IoDriver.h" 
#include <cstring>
#include <iostream>

bool isValidIP(std::string ip)
{
	if (strlen(ip.c_str()) != strlen("???.???.???.???"))
		return false;
	if (ip[3] != '.' || ip[7] != '.' || ip[11] != '.')
		return false;
	
	return true;
}

Order::Order()
	: direction(BUTTON_COMMAND)
	, floor(-1)
	, assignedIP("")
	, timeAssigned(-1)
{}

Order::Order(	button_type_t direction, 
				int floor, 
				std::string assignedIP, 
				time_t timeAssigned)
	: direction(direction)
	, floor(floor)
	, assignedIP(assignedIP)
	, timeAssigned(timeAssigned)
{}

bool Order::isValid() 
{
	if (direction != BUTTON_COMMAND 
		&& direction != BUTTON_CALL_UP 
		&& direction != BUTTON_CALL_DOWN)
		return false;

	if (floor < 0 || floor >= N_FLOORS)
		return false;

	if (!isValidIP(assignedIP))
		return false;

	return true;
}

bool Order::operator==(const Order& rhs) const
{
	if (direction == rhs.direction
		&& floor == rhs.floor
		&& assignedIP == rhs.assignedIP)
		return true;
	else
		return false;
}

bool Order::operator<(const Order& rhs) const
{
	if (floor < rhs.floor) 
		return true;
	else if (floor > rhs.floor)
		return false;

	if (direction < rhs.direction) 
		return true;
	else if (direction > rhs.direction)			
		return false;
	
	if (assignedIP < rhs.assignedIP)
		return true;
	else
		return false;
}


Offer::Offer()
	: cost(-1)
	, floor(-1)
	, direction(BUTTON_COMMAND)
	, fromIP("")
{}

Offer::Offer(	int cost, 
				int floor, 
				button_type_t direction, 
				std::string fromIP)
	: cost(cost)
	, floor(floor)
	, direction(direction)
	, fromIP(fromIP)
{}

bool Offer::isValid()
{
	if (cost < 0)
		return false;
	if (floor < 0 || floor >= N_FLOORS)
		return false;
	if (direction != BUTTON_COMMAND 
		|| direction != BUTTON_CALL_UP 
		|| direction != BUTTON_CALL_DOWN)
		return false;
	if (!isValidIP(fromIP))
		return false;

	return true;
}

bool Offer::operator < (const Offer& rhs) const
{
	if (cost < rhs.cost) 
		return true;
	else
		return false;
}











