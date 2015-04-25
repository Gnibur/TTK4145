#include "OrderManager.h"
#include "IoDriver.h"
#include "msgTool.h"
#include "MsgParser.h"
#include "udp.h"
#include <ctime>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <fstream>
#include <pthread.h>


OrderList orderList;


static pthread_mutex_t orderManagerMutex;

bool saveOrderList(std::string filename);
bool retrieveOrderList(std::string filename);

void orderManager_recover()
{
	pthread_mutex_init(&orderManagerMutex, NULL);

	if (!retrieveOrderList("Backup1.txt"))
		retrieveOrderList("Backup2.txt");			

}


bool saveOrderList(std::string filename)
{
	std::ofstream backupFile;

	backupFile.open(filename);

	if (!backupFile.is_open())
		return false;
	
	pthread_mutex_lock(&orderManagerMutex);

	backupFile << msgParser_makeOrderListMsg(orderList, udp_myIP());	

	pthread_mutex_unlock(&orderManagerMutex);

	backupFile.close();

	return true;
}

bool retrieveOrderList(std::string filename)
{
	std::fstream backupFile;
	backupFile.open(filename);

	if (!backupFile)
		return false;

	std::string content, temp;
	while (getline(backupFile, temp))
		content += temp;
	backupFile.close();
		
	
	return msgParser_getOrderListFromMessage(content, &orderList);
}


bool orderManager_addOrder(Order order)
{
	bool orderAdded = false;
	pthread_mutex_lock(&orderManagerMutex);
	std::vector<Order>::iterator search = std::find(orderList.begin(), orderList.end(), order);
	
	if (search == orderList.end())
	{
		orderList.push_back(order);
		orderAdded = true;
	}

	pthread_mutex_unlock(&orderManagerMutex);
	
	
	if (!(saveOrderList("Backup1.txt") || saveOrderList("Backup2.txt")) )
		return false;

	if ((order.direction == BUTTON_COMMAND && order.assignedIP == udp_myIP()) || order.direction != BUTTON_COMMAND)
		ioDriver_setOrderButtonLamp(order.direction, order.floor);

	return orderAdded;
}

bool orderManager_clearOrder(Order order)
{
	bool orderCleared = false;
	
	pthread_mutex_lock(&orderManagerMutex);
	std::vector<Order>::iterator search = std::find(orderList.begin(), orderList.end(), order);

	if (search != orderList.end())
	{
		orderList.erase(search);
		orderCleared = true;
	}


	pthread_mutex_unlock(&orderManagerMutex);
	
	if (!(saveOrderList("Backup1.txt") || saveOrderList("Backup2.txt")) )
		return false;
	
	if ((order.direction == BUTTON_COMMAND && order.assignedIP == udp_myIP()) || (order.direction != BUTTON_COMMAND))
		ioDriver_clearOrderButtonLamp(order.direction, order.floor);

	return orderCleared;
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
		if (it->floor == floor && it->assignedIP == udp_myIP())
			returnList.push_back(*it);
	}
	pthread_mutex_unlock(&orderManagerMutex);	
	return returnList;
}


bool orderManager_shouldStopHere(int floor, motor_direction_t direction) 
{
	bool hasOrderHere = false;
	bool hasOrderHereInSameDirection = false;
	bool hasMoreOrdersInSameDirection = false;

	for (auto orderPtr = orderList.begin(); orderPtr != orderList.end(); ++orderPtr){
		
		if (orderPtr->floor == floor){
			hasOrderHere = true;
			
			if (orderPtr->direction == BUTTON_COMMAND)
				return true;

			if (orderPtr->direction != BUTTON_COMMAND){

				if (direction == DIRECTION_UP && orderPtr->direction == BUTTON_CALL_UP)
					hasOrderHereInSameDirection = true;
				else if (direction == DIRECTION_DOWN && orderPtr->direction == BUTTON_CALL_DOWN)
					hasOrderHereInSameDirection = true;
			}

		}
		
		if (direction == DIRECTION_UP && orderPtr->floor > floor)
			hasMoreOrdersInSameDirection = true;
		else if (direction == DIRECTION_DOWN && orderPtr->floor < floor)
			hasMoreOrdersInSameDirection = true;

		if (hasOrderHereInSameDirection)
			break;
	}
	 	
	if (hasOrderHereInSameDirection)
		return true;
	else if (hasOrderHere && !hasMoreOrdersInSameDirection)
		return true;
	else
		return false;
}



motor_direction_t orderManager_getNextDirection(int floor, motor_direction_t lastDirection)
{
	OrderList myPendingOrders;
	for (auto orderPtr = orderList.begin(); orderPtr != orderList.end(); ++orderPtr)
		if (orderPtr->assignedIP == udp_myIP())
			myPendingOrders.push_back(*orderPtr);

	if (myPendingOrders.size() == 0) 
		return DIRECTION_STOP;

	for (auto orderPtr = myPendingOrders.begin(); orderPtr != myPendingOrders.end(); ++orderPtr)
		if (orderPtr->floor == floor)
			return DIRECTION_STOP;

	if (myPendingOrders.begin()->floor > floor)
		return DIRECTION_UP;
	else
		return DIRECTION_DOWN;	

	bool hasOrderInSameDirection = false;
	for (auto orderPtr = myPendingOrders.begin(); orderPtr != myPendingOrders.end(); ++orderPtr){
		if (lastDirection == DIRECTION_UP)
		{
			if (floor < orderPtr->floor)
				hasOrderInSameDirection = true;
		}
		else if (lastDirection == DIRECTION_DOWN)
		{
			if (floor > orderPtr->floor)
				hasOrderInSameDirection = true;
		}	
	}

	if (hasOrderInSameDirection)
		return lastDirection;
	else {
		if (lastDirection == DIRECTION_UP)
			return DIRECTION_DOWN;
		else 
			return DIRECTION_UP;
	}	
}

int orderManager_getCost(int lastFloor, int newFloor, motor_direction_t lastDirection, button_type_t wantedDirection)
{
	int cost = abs(lastFloor - newFloor);
	motor_direction_t wantedMotorDirection;
	
	// Regular cost function
	if (wantedDirection == BUTTON_CALL_UP)
		wantedMotorDirection = DIRECTION_UP;
	else if (wantedDirection == BUTTON_CALL_DOWN)
		wantedMotorDirection = DIRECTION_DOWN;
	else
		wantedMotorDirection = DIRECTION_STOP;

	if (((lastDirection == DIRECTION_UP) && (newFloor < lastFloor)) || ((lastDirection == DIRECTION_DOWN) && (newFloor > lastFloor)))
	{
		cost += N_FLOORS * 2;
		if (wantedMotorDirection == lastDirection)
			cost += N_FLOORS;
	}
	else
	{
		if (lastDirection != wantedMotorDirection)
			cost += N_FLOORS;
	}

	// Expansion for the cost function, so one elevator doesn't take all.
	for (auto it = orderList.begin(); it != orderList.end(); ++it)
	{
		if (it->assignedIP.compare(udp_myIP()) == 0)
			cost += 2;
	}

	return cost;
}

void orderManager_mergeMyOrdersWith(OrderList orders)
{
	pthread_mutex_lock(&orderManagerMutex);
	
	for (auto it = orders.begin(); it != orders.end(); ++it)
	{
		if (std::find(orderList.begin(), orderList.end(), (*it)) == orderList.end())
		{
			std::cout << "Pushing back this with IP: " << it->assignedIP << std::endl;
			it->timeAssigned = time(0);
			orderList.push_back((*it));
		if ((it->direction == BUTTON_COMMAND && it->assignedIP == udp_myIP()) || (it->direction != BUTTON_COMMAND))
			ioDriver_setOrderButtonLamp(it->direction, it->floor);
		}
	}
	msgTool_sendOrderList(orderList, udp_myIP());
	
	pthread_mutex_unlock(&orderManagerMutex);	

	if (!(saveOrderList("Backup1.txt") || saveOrderList("Backup2.txt")) )
		std::cout << "Failed saving merged orders\n";
}

bool orderManager_orderListEquals(OrderList rhs)
{
	pthread_mutex_lock(&orderManagerMutex);
	std::sort(rhs.begin(), rhs.end());
	std::sort(orderList.begin(), orderList.end());
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
		if ((it->timeAssigned + ORDER_TIMEOUT) < time_now) return (*it);
	}
	Order defaultOrder;
	return defaultOrder;
}
