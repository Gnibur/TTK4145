#include "OrderManager.h"
#include "udp.h"
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
		orderList.push_back(order);
	
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
	OrderList returnList;
	for (auto it = orderList.begin(); it != orderList.end(); ++it)
	{
		if ((it->floor == floor) && (it->assignedIP.compare(getMyIP()) == 0))
			returnList.push_back(*it);
	}
	pthread_mutex_unlock(&orderManagerMutex);	
	return returnList;
}

motor_direction_t orderManager_getNextDirection(int floor, motor_direction_t lastDirection)
{
	// If there are no orders, don't move
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
		if ((orderedFloor > floor) && (it->assignedIP.compare(getMyIP()) == 0))
			return lastDirection;
	}

	// If you don't find anything in your direction ,change.
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
	bool returnVal = true;
	while (lhsIterator != orderList.end())
	{
		if (rhsIterator == rhs.end())
			returnVal = false;
		if (!((*lhsIterator) == (*rhsIterator)))
			returnVal = false;

		++lhsIterator;
		++rhsIterator;
	}
	if (rhsIterator != rhs.end())
		returnVal = false;
	pthread_mutex_unlock(&orderManagerMutex);	
	return returnVal;
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
