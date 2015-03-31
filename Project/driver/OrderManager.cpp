#include "OrderManager.h"
#include <stdlib.h>
#include <algorithm>
#include <ctime>
#include <iostream>

// REMOVE THIS!!!!!!!
#define N_FLOORS 4

OrderManager::OrderManager()
{
	myID = 1;
}

Order OrderManager::getOrderWithLowestCost(int currentFloor, order_direction_t currentDirection, OrderList stashedList)
{

	OrderList searchList;
	std::sort(stashedList.begin(), stashedList.end()); 	// TODO: Remove if redundant
	
	// Set ut searchlist so it does not containt the elements of stashedlist.
	std::set_difference(
			orderList.begin(), orderList.end(),
			stashedList.begin(), stashedList.end(),
			std::back_inserter( searchList )
	);
	
	int directionMultiplier = (currentDirection == DIRECTION_UP)? 1 : -1; // Multiply with the floors when checking if it's in our wanted direction, so we can treat both directions the same
	int multipliedCurrentFloor = currentFloor * directionMultiplier;
	int lowestCost = (N_FLOORS * 4);
	Order bestOrder;

	for (auto it = orderList.begin(); it != orderList.end(); ++it)
	{
		if ((it->elevator != 0) && (it->elevator != myID))
			continue;

		int multipliedFloor = it->floor * directionMultiplier;
		int cost = abs(it->floor - currentFloor);
		// Case 1: The floor is in our direction
		if (multipliedFloor > multipliedCurrentFloor)
		{
			if (currentDirection != it->direction)
				cost += N_FLOORS; // Worst case: The ordered direction is the opposite of what we're going
		}

		// Case 2: The floor is NOT in our direction
		else
		{
			// This is already a worse case than any of the previous, so the cost needs to be bigger
			cost += (2 * N_FLOORS);
			if (currentDirection == it->direction)
				cost += N_FLOORS; // Worst case: The ordered direction is the direction we're going
		}

		if (cost < lowestCost)
		{
			lowestCost = cost;
			bestOrder = (*it);
		}
	}
	return bestOrder;
}

int OrderManager::updateList(order_status status, OrderList list, Order order) {
	// TODO: Find out how to handle the searching when the status is UPDATE
	std::vector<Order>::iterator search = std::find(orderList.begin(), orderList.end(), order);
	if (status == NEW)
	{
		if (search == orderList.end()) orderList.push_back(order);
	}
	if (status == DELETE)
	{
		if (search != orderList.end()) orderList.erase(search);
	}
	if (status == UPDATE)
	{
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			if (std::find(orderList.begin(), orderList.end(), (*it)) == orderList.end()) orderList.push_back((*it));
		}
	}
	std::sort(orderList.begin(), orderList.end());
	
	if ((status == DELETE) || (status == NEW))
	{
		// TODO: Check if the lists are the same, then shout out for an update
	}
	return 0;
}

// TEST FUNCTIONS
void OrderManager::printOrders () {
	std::cout << "Floor \t Dir \t Elev \n";
	for (auto it = orderList.begin(); it != orderList.end(); ++it)
	{
		std::cout << it->floor << "\t" << it->direction << "\t" << it->elevator << "\n";
	}
}
