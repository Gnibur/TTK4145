#include "OrderManager.h"
#include <ctime>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <pthread.h>


OrderList orderList;


static pthread_mutex_t orderManagerMutex;

void orderManager_init()
{
	pthread_mutex_init(&orderManagerMutex, NULL);
}



void orderManager_newOrder(Order order)
{
	pthread_mutex_lock(&orderManagerMutex);
	std::vector<Order>::iterator search = std::find(orderList.begin(), orderList.end(), order);
	if (search == orderList.end())
	{
		std::cout << "Pushing back new order :) \n\n";
		orderList.push_back(order);
	}
	std::sort(orderList.begin(), orderList.end());
	pthread_mutex_unlock(&orderManagerMutex);	
}

void orderManager_clearOrder(Order order)
{
	pthread_mutex_lock(&orderManagerMutex);
	std::vector<Order>::iterator search = std::find(orderList.begin(), orderList.end(), order);
	if (search != orderList.end())
		orderList.erase(search);
	std::sort(orderList.begin(), orderList.end());
	pthread_mutex_unlock(&orderManagerMutex);	
}

OrderList orderManager_getOrders()
{
	return orderList;
}

OrderList orderManager_getOrdersOnFloor(int floor)
{
	pthread_mutex_lock(&orderManagerMutex);
	std::cout << "Now soon doing the for-dance\n";	
	OrderList returnList;
	for (auto it = orderList.begin(); it != orderList.end(); ++it)
	{	
		std::cout << "Searching for floor: " << floor << std::endl;
		std::cout << "Found floor: " << it->floor << std::endl;
		// && (it->assignedIP == IP)
		if (it->floor == floor)
			returnList.push_back(*it); std::cout << "Adding...?\n";
	}
	pthread_mutex_unlock(&orderManagerMutex);	
	return returnList;
}

motor_direction_t orderManager_getNextDirection(int floor, motor_direction_t lastDirection)
{
	if (orderList.empty()) return DIRECTION_STOP;

	// If the elevator is idle, prioritize the floors closest
	if (lastDirection == DIRECTION_STOP)
	{
		if (floor >= 2) lastDirection = DIRECTION_UP;
		else			lastDirection = DIRECTION_DOWN;
	}

	// Find the directional multiplier, so we can use the same check whether you are going up or down.
	int directionalMultiplier;
	motor_direction_t newDirection;
	if (lastDirection == DIRECTION_UP)
	{
		directionalMultiplier = 1;
		newDirection = DIRECTION_DOWN;
	}
	else
	{
		directionalMultiplier = -1;
		newDirection = DIRECTION_UP;
	}
	
	floor = directionalMultiplier * floor;

	// Go through all the elements. If you find something in your direction, imideatly return.
	for (auto it = orderList.begin(); it != orderList.end(); ++it)
	{
		int orderedFloor = it->floor * directionalMultiplier;
		if (orderedFloor > floor)
			return lastDirection;
	}

	// If you get this far, you need to change the direction.
	return newDirection;
}

int orderManager_getCost(int lastFloor, int newFloor, motor_direction_t lastDirection, button_type_t wantedDirection)
{
	motor_direction_t motorDirection;
	if (wantedDirection == BUTTON_CALL_DOWN)
		motorDirection = DIRECTION_DOWN;
	else if (wantedDirection == BUTTON_CALL_UP)
		motorDirection = DIRECTION_UP;
	else
		motorDirection = DIRECTION_STOP;

	int cost = abs(lastFloor - newFloor);
	
	// In case of opposite direction
	if (((newFloor < lastFloor) && (lastDirection == DIRECTION_UP)) || ((newFloor > lastFloor) && (lastDirection == DIRECTION_DOWN)))
	{
		cost += N_FLOORS;
	}
	return cost;
}

void orderManager_mergeMyOrdersWith(OrderList orders)
{
	pthread_mutex_lock(&orderManagerMutex);
	std::cout << "Entering merge my orders with..\n\n";
	for (auto it = orders.begin(); it != orders.end(); ++it)
	{
		if (std::find(orderList.begin(), orderList.end(), (*it)) == orderList.end())
			orderList.push_back((*it));
	}
	std::sort(orderList.begin(), orderList.end());
	pthread_mutex_unlock(&orderManagerMutex);	
}

bool orderManager_orderListEquals(OrderList rhs)
{
	pthread_mutex_lock(&orderManagerMutex);
	auto rhsIterator = rhs.begin();
	auto lhsIterator = orderList.begin();
	while (lhsIterator != orderList.end())
	{
		if (rhsIterator == rhs.end())				return false;
		if (!((*lhsIterator) == (*rhsIterator)))	return false;

		++lhsIterator;
		++rhsIterator;
	}
	if (rhsIterator != rhs.end())				return false;
	pthread_mutex_unlock(&orderManagerMutex);	
	return true;
}

Order orderManager_checkForOrderTimeout()
{
	for (auto it = orderList.begin(); it != orderList.end(); ++it)
	{ 
		time_t timer;
		int time_now = time(&timer);
		if ((it->timeAssigned - ORDER_TIMEOUT) < time_now) return (*it);
	}
	Order defaultOrder;
	return defaultOrder;
}
