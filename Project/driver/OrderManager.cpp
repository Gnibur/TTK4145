#include "OrderManager.h"
#include <stdlib.h>
#include <ctime>
#include <iostream>

// REMOVE THIS!!!!!!!
#define N_FLOORS 4

OrderManager::OrderManager()
{
	myID = 1;
	Order order[10];
	order[0] = {0, DIRECTION_UP, 0};
	order[1] = {1, DIRECTION_DOWN, 0};
	order[2] = {1, DIRECTION_UP, 0};
	order[3] = {2, DIRECTION_UNDEFINED, 1};
	order[4] = {2, DIRECTION_UNDEFINED, 2};
	order[5] = {3, DIRECTION_DOWN, 0};
	order[6] = {3, DIRECTION_UNDEFINED, 3};
	order[7] = {0, DIRECTION_UNDEFINED, 1};
	order[8] = {1, DIRECTION_UNDEFINED, 2};
	order[9] = {2, DIRECTION_UP, 0};

	for (int i = 0; i < 10; i++)
	{
		orderList.push_back(order[i]);
	}
}

int OrderManager::getFloorWithLowestCost(int currentFloor, order_direction_t currentDirection)
{
	int directionMultiplier = (currentDirection == DIRECTION_UP)? 1 : -1; // Multiply with the floors when checking if it's in our wanted direction, so we can treat both directions the same
	int multipliedCurrentFloor = currentFloor * directionMultiplier;
	int lowestCost = (N_FLOORS * 4);
	int bestFloor = -1;

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
			bestFloor = it->floor;
		}
	}
	return bestFloor;
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
		{
			//Case 3.1: The floor is in our direction
			if (relativeCurrentFloor < relativeFloor)
			{
				cost = distance + N_FLOORS + distance;
			}
			// Case 3.1: The floor is NOT in our direction
			else
			{
				cost = distance + (2 * N_FLOORS) + distance;
			}
		}
		// Check if this cost is the lowest
		if (cost < lowestCost)
		{
			lowestCost = cost;
			bestFloor = it->floor;
		}
	}
	return bestFloor;
}*/
