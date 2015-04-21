//#include "Globals.h"
#include "OrderManager.h"
#include <ctime>
#include <algorithm>
#include <cmath>

OrderList orderList;
static std::string IP = "102.01.01.105";

void orderManager_newOrder(Order order)
{
	std::vector<Order>::iterator search = std::find(orderList.begin(), orderList.end(), order);
	if (search == orderList.end())
	{
		orderList.push_back(order);
	}
	std::sort(orderList.begin(), orderList.end());
}

void orderManager_clearOrder(Order order)
{
	std::vector<Order>::iterator search = std::find(orderList.begin(), orderList.end(), order);
	if (search != orderList.end())
		orderList.erase(search);
	std::sort(orderList.begin(), orderList.end());
}

OrderList orderManager_getOrders()
{
	return orderList;
}

OrderList orderManager_getOrdersOnFloorInDirection(int floor, button_type_t direction)
{
	OrderList returnList;
	for (auto it = orderList.begin(); it != orderList.end(); ++it)
	{	// && (it->assignedIP == IP)
		if ((it->floor == floor)  && ((it->direction == direction) || (it->direction == BUTTON_COMMAND) || (it->floor == N_FLOORS - 1) || (it->floor == 0)))
			returnList.push_back(*it);
	}
	return returnList;
}

motor_direction_t orderManager_getNextDirection(int floor, motor_direction_t lastDirection)
{
	// If the list is empty, stand still.
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
	// Case: In the same direction
	if (((newFloor > lastFloor) && (lastDirection == DIRECTION_UP)) || ((newFloor < lastFloor) && (lastDirection == DIRECTION_DOWN)))
	{
		// Subcase: Wanting to go the other direction
		if ((motorDirection != lastDirection) && (motorDirection != DIRECTION_STOP))
			cost += N_FLOORS * 1;
	}
	// Case: You need to change direction
	else
	{
		// Subcase: You need to change direction _again_
		if ((motorDirection == lastDirection) && (motorDirection != DIRECTION_STOP))
			cost += N_FLOORS * 3;
		// Subcase: You only need to change direction once.
		else
			cost += N_FLOORS * 2;
	}
	return cost;
}

void orderManager_mergeMyOrdersWith(OrderList orders)
{
	for (auto it = orders.begin(); it != orders.end(); ++it)
	{
		if (std::find(orderList.begin(), orderList.end(), (*it)) == orderList.end())
			orderList.push_back((*it));
	}
	std::sort(orderList.begin(), orderList.end());
}

bool orderManager_orderListEquals(OrderList rhs)
{
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
