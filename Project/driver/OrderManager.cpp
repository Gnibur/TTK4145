//#include "Globals.h"
#include "OrderManager.h"
#include <ctime>
#include <algorithm>
#include <cmath>


void OrderManager::newOrder(int floor, button_type_t direction)
{
	time_t timer;
	int assignedDate = time(&timer);
	Order order = {direction, floor, IP, assignedDate };
	std::vector<Order>::iterator search = std::find(orderList.begin(), orderList.end(), order); // See if the order is already there
	if (search == orderList.end())
	{
		orderList.push_back(order);
	}
	std::sort(orderList.begin(), orderList.end());
}

void OrderManager::newOrder(Order order)
{
	std::vector<Order>::iterator search = std::find(orderList.begin(), orderList.end(), order);
	if (search == orderList.end())
	{
		orderList.push_back(order);
	}
	std::sort(orderList.begin(), orderList.end());
}

void OrderManager::clearOrder(Order order)
{
	std::vector<Order>::iterator search = std::find(orderList.begin(), orderList.end(), order);
	if (search != orderList.end())
		orderList.erase(search);
	std::sort(orderList.begin(), orderList.end());
}

void OrderManager::clearOrders(OrderList orders)
{
	for (auto it = orders.begin(); it != orders.end(); ++it)
	{
		orderList.erase(it);
	}
	std::sort(orderList.begin(), orderList.end());
}

OrderList OrderManager::getOrdersOnFloorInDirection(int floor, button_type_t direction);
{
	OrderList returnList;
	for (auto it = orderList.begin(); it != orderList.end(); ++it)
	{
		if ((it->floor == floor) && (it->assignedIP == IP) && ((it->direction == direction) || (it->direction == BUTTON_COMMAND)))
			returnList.push_back(*it);
	}
	return returnList;
}

motor_direction_t OrderManager::getNextDirection(int floor, motor_direction_t lastDirection)
{
	// If the list is empty, stand still.
	if (orderList.empty()) return DIRECTION_STOP;

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

int OrderManager::getCost(int lastFloor, int newFloor, motor_direction_t lastDirection, button_type_t wantedDirection)
{
	int cost = abs(lastFloor - newFloor);
	// Case: In the same direction
	if (((newFloor > lastFloor) && (lastDirection == DIRECTION_UP)) || ((newFloor < lastFloor) && (lastDirection == DIRECTION_DOWN)))
	{
		// Subcase: Wanting to go the other direction
		if (((motor_direction_t)wantedDirection != lastDirection) && (wantedDirection != ORDER_INSIDE))
			cost += N_FLOORS * 1;
	}
	// Case: You need to change direction
	else
	{
		// Subcase: You need to change direction _again_
		if (((motor_direction_t)wantedDirection == lastDirection) && (wantedDirection != ORDER_INSIDE))
			cost += N_FLOORS * 3;
		// Subcase: You only need to change direction once.
		else
			cost += N_FLOORS * 2;
	}
	return cost;
}

void OrderManager::mergeMyOrdersWith(OrderList orders)
{
	for (auto it = orders.begin(); it != orders.end(); ++it)
	{
		if (std::find(orderList.begin(), orderList.end(), (*it)) == orderList.end())
			orderList.push_back((*it));
	}
	std::sort(orderList.begin(), orderList.end());
}

int main ()
{
	return 0;
}
