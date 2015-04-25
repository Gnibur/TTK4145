#pragma once

#include "Enums.h"
#include <vector>
#include <string>
#include <time.h>

bool isValidIP(std::string ip);


class Order {
public:
	button_type_t direction;
	int floor;
	std::string assignedIP;
	time_t timeAssigned;

	Order(); 
	Order(	button_type_t direction, 
			int floor, 
			std::string assignedIP, 
			time_t timeAssigned = -1);

	bool isValid();

	bool operator==(const Order& rhs) const;
	
	bool operator < (const Order& rhs) const;
};

typedef std::vector<Order> OrderList;

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

