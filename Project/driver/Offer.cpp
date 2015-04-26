#include "AuctionManager.h"
#include "udp.h"
#include <cstring>


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
	if (floor < 0 || floor >= FLOORCOUNT)
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



