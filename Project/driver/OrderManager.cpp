#include "OrderManager.h"
#include <stdlib.h>
<<<<<<< HEAD
#include <algorithm>
=======
#include <ctime>
#include <iostream>

// REMOVE THIS!!!!!!!
#define N_FLOORS 4
>>>>>>> 8976588fdf22b93052b5f4a131bf6edeff400342

OrderManager::OrderManager()
{
	myID = 1;
}

<<<<<<< HEAD
int OrderManager::getFloorWithLowestCost(int currentFloor, motor_direction_t currentDirection, OrderList stashedList)
{
	OrderList searchList;
	std::sort(orderList.begin(), orderList.end()); 		// TODO: Remove if redundant
	std::sort(stashedList.begin(), stashedList.end()); 	// TODO: Remove if redundant
	
	// Set ut searchlist so it does not containt the elements of stashedlist.
	std::set_difference(
			orderList.begin(), orderList.end(),
			stashedList.begin(), stashedList.end(),
			std::back_inserter( searchList )
	);
	
	// Multiply with the floors when checking if it's in our wanted direction, so we can treat both directions the same
	int directionMultiplier = (currentDirection == DIRECTION_UP)? 1 : -1;
	int multipliedCurrentFloor = currentFloor * multiplier;
	int lowestCost = N_FLOOR * 4;
	int bestFloor = -1;
	
	for (auto it = searchList.begin(); it != orderList.end(); ++it)
=======
int OrderManager::getFloorWithLowestCost(int currentFloor, order_direction_t currentDirection)
{
	int directionMultiplier = (currentDirection == DIRECTION_UP)? 1 : -1; // Multiply with the floors when checking if it's in our wanted direction, so we can treat both directions the same
	int multipliedCurrentFloor = currentFloor * directionMultiplier;
	int lowestCost = (N_FLOORS * 4);
	int bestFloor = -1;

	for (auto it = orderList.begin(); it != orderList.end(); ++it)
>>>>>>> 8976588fdf22b93052b5f4a131bf6edeff400342
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
			bestFloor = it->floor;
		}
	}
	return bestFloor;
}

<<<<<<< HEAD
int OrderManager::updateList() {
	
}

/*
		int cost;
		int distance = abs(it->floor - currentFloor);
		// Case 1: The order has a specified direction, which is the direction we're going
		if (order_direction_t::it->direction == currentDirection)
		{
			// Case 1.1: The floor is in our direction
			if (relativeCurrentFloor < relativeFloor)
			{
				cost = distance;
			}
			// Case 1.2: The floor is NOT in our direction
			else
			{
				cost = (3 * N_FLOORS) + distance;
			}
		}
		// Case 2: The floor direction is undefined
		else if (order_direction_t::it->direction == DIRECTION_UNDEFINED)
		{
			// Case 2.1: The floor is in our direction
			if (relativeCurrentFloor < relativeFloor)
			{
				cost = distance;
			}
			// Case 2.1: The floor is NOT in our direction
			else 
			{
				cost = (2 * N_FLOORS) + distance;
			}
		}
		// Case 3: The order has a specified direction, which is the OPPOSITE direction we're going
		else
=======
int OrderManager::updateList(int status, OrderList list, Order order = {}) {
	if (status == 0) // New
	{
		orderList.push_back(order);
	}
	if (status == 1) // Delete
	{
		std::sort(orderList.begin(), orderList.end());
		auto it = std::find(orderList.begin(), orderList.end(), order);
		if (it != orderList.end())
>>>>>>> 8976588fdf22b93052b5f4a131bf6edeff400342
		{
			orderList.erase(it);
		}
	}
}

/*
int OrderManager::mergeList() {
	OrderList oldList = orderList;
	orderList
}*/


// TEST FUNCTIONS
void OrderManager::printOrders () {
	std::cout << "Floor \t Dir \t Elev \n";
	for (auto it = orderList.begin(); it != orderList.end(); ++it)
	{
		std::cout << it->floor << "\t" << it->direction << "\t" << it->elevator << "\n";
	}
}
