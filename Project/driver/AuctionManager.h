#pragma once

#include "GlobalDefines.h"
#include <string>



void auction_start(int floor, button_type_t direction);


class Offer {
public:
	int cost;
	int floor;
	button_type_t direction;
	std::string fromIP;

	Offer();

	Offer(	int cost, 
			int floor, 
			button_type_t direction, 
			std::string fromIP);


	bool isValid();

	bool operator < (const Offer& rhs) const;
};


void auction_addBid(Offer offer);
