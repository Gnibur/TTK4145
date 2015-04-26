/*
	OrderManager holds Order's in memory and on disk.
	Holding the orderList it can decide where to move, 
	and which orders are expensive.
*/


#pragma once
#include <vector>
#include <string>


#include "GlobalDefines.h"


class Order {
public:
	int floor;
	button_type_t direction;
	std::string assignedIP;
	time_t timeAssigned;

	Order(); 
	Order(int floor, button_type_t direction, std::string assignedIP);

	bool isValid();	

	bool operator<(const Order& rhs) const;
	bool operator==(const Order& rhs) const;
};

typedef std::vector<Order> OrderList;

void orderManager_recoverFromDisk();

#define SEND_UPDATE 		true
#define DONT_SEND_UPDATE 	false

bool orderManager_addOrder(Order order, 	bool sendupdate);
bool orderManager_clearOrder(Order order, 	bool sendupdate);
bool orderManager_clearOrdersAt(int floor, std::string orderIP, bool sendupdate);
void orderManager_mergeOrderListWith(OrderList orders, bool sendupdate);

// Returns a order which is timed out
Order orderManager_checkForOrderTimeout();

bool orderManager_shouldElevatorStopHere(int floor, motor_direction_t direction); 
motor_direction_t orderManager_getNextMotorDirection(int floor, motor_direction_t lastDirection);

// When the function returns a high value, it takes long time to travel there. 
int	orderManager_getOrderCost(int orderFloor, button_type_t orderDirection, int lastFloor, motor_direction_t lastDirection);


