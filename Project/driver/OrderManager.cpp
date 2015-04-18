#include "OrderManager_new.h" // Change to OrderManager.h
#include <ctime>
#include "Globals.h"
#include <algorithm>

void OrderManager::newOrder(int floor, order_direction_t direction)
{
	time_t timer;
	Order newOrder = {floor, direction, IP, time(&timer) };
	std::sort(stashedList.begin(), stashedList.end());
	std::vector<Order>::iterator search = std::find(orderList.begin(), orderList.end(), newOrder); // See if the order is already there
	if (search == orderList.end())
		orderList.push_back(newOrder);
}

void OrderManager::clearOrder(Order order)
{
	std::vector<Order>::iterator search = std::find(orderList.begin(), orderList.end(), order);
	if (search != orderList.end())
		orderList.erase(search);
}

bool OrderManager::hasOrderOnFloor(int floor)
{
	for (auto it = orderList.begin(); it != orderList.end(); ++it)
	{
		if (it->floor == floor)
			return true;
	}
	return false;
}

motor_direction_t OrderManager::getNextDirection(int floor, motor_direction_t lastDirection)
{
	// If the list is empty, stand still.
	if (orderList.empty()) return DIRECTION_STOP;

	// Find the directional multiplier, so we can use the same check whether you are going up or down.
	int directionalMultiplier;
	motor_direction_t = newDirection;
	if (lastDirection == UP)
		directionalMultiplier = 1;
		newDirection = DIRECTION_DOWN;
	else
		directionalMultiplier = -1;
		newDirection = DIRECTION_UP;

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

int OrderManager::getCost(int floor)
{

}

int mergeOrdersWith(OrderList orders)
{

}