#include "OrderManager.h"
#include "udp.h"


Order::Order()
	: floor(-1)
	, direction(BUTTON_COMMAND)
	, assignedIP("")
	, timeAssigned(-1)
{}

Order::Order(int floor, button_type_t direction, std::string assignedIP)
	: floor(floor)
	, direction(direction)
	, assignedIP(assignedIP)
{}

bool Order::isValid() 
{
	if (direction != BUTTON_COMMAND 
		&& direction != BUTTON_CALL_UP 
		&& direction != BUTTON_CALL_DOWN)
		return false;

	if (floor < 0 || floor >= FLOORCOUNT)
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

