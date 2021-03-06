#include "OrderManager.h"
#include "IoDriver.h"
#include "MsgParser.h"
#include "udp.h"
#include <ctime>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <fstream>
#include <pthread.h>


static OrderList orderList;


static pthread_mutex_t orderManagerMutex = PTHREAD_MUTEX_INITIALIZER;

static bool saveOrderList(std::string filename, OrderList order);
static bool retrieveOrderList(std::string filename, OrderList *orders);

void orderManager_recover()
{
	pthread_mutex_lock(&orderManagerMutex);	
	if (!retrieveOrderList("Backup1.txt", &orderList))
		retrieveOrderList("Backup2.txt", &orderList);

	std::string orderListMsg;
	orderListMsg = msgParser_makeOrderListMsg(orderList, getMyIP());
	udp_send(orderListMsg.c_str(), strlen(orderListMsg.c_str()) + 1);	
			
	pthread_mutex_unlock(&orderManagerMutex);		
}

bool saveOrderList(std::string filename, OrderList orders)
{
	std::ofstream backupFile;

	backupFile.open(filename);

	if (!backupFile.is_open())

		return false;

	backupFile << msgParser_makeOrderListMsg(orders, getMyIP());	

	backupFile.close();

	// acceptance test
	OrderList tempOrders;
	return retrieveOrderList(filename, &tempOrders);
}

bool retrieveOrderList(std::string filename, OrderList *orders)
{
	std::fstream backupFile;
	backupFile.open(filename);

	if (!backupFile)
		return false;

	std::string content, temp;
	while (getline(backupFile, temp))
		content += temp;
	backupFile.close();
		
	
	return msgParser_getOrderListFromMessage(content, orders);
}


bool orderManager_addOrder(Order order, bool sendupdate)
{
	bool transactionSucceeded = false;	
	
	bool orderAddedToList = false;
	bool orderAddedToStorage = false;

	pthread_mutex_lock(&orderManagerMutex);
	OrderList newOrderList = orderList;

	OrderList::iterator search = std::find(newOrderList.begin(), newOrderList.end(), order);
	
	if (search == newOrderList.end())
	{
		order.timeAssigned = time(0);
		newOrderList.push_back(order);
		orderAddedToList = true;
	} else {
		search->timeAssigned = time(0);
		orderAddedToList = true;
	}

	if (saveOrderList("Backup1.txt", newOrderList) || saveOrderList("Backup2.txt", newOrderList))
		orderAddedToStorage = true;
	else {
		saveOrderList("Backup1.txt", orderList);
		saveOrderList("Backup2.txt", orderList);
	}

	if (orderAddedToList && orderAddedToStorage)
	{
		orderList = newOrderList;


		if (sendupdate == true){
			std::string msg;
			msg = msgParser_makeNewOrderMsg(order, newOrderList, getMyIP());
			udp_send(msg.c_str(), strlen(msg.c_str()) + 1);
		}

		if ((order.direction == BUTTON_COMMAND && order.assignedIP == getMyIP()) || order.direction != BUTTON_COMMAND)
			ioDriver_setOrderButtonLamp(order.direction, order.floor);
		
		transactionSucceeded = true;

	}	

	pthread_mutex_unlock(&orderManagerMutex);
	return transactionSucceeded;

}

bool orderManager_clearOrder(Order order, bool sendupdate)
{
	bool transactionSucceeded = false;

	bool orderClearedFromList = false;
	bool orderClearedFromStorage = false;
	
	pthread_mutex_lock(&orderManagerMutex);
	OrderList newOrderList = orderList;


	OrderList::iterator search = std::find(newOrderList.begin(), newOrderList.end(), order);
	
	if (search != newOrderList.end())
	{
		newOrderList.erase(search);
		orderClearedFromList = true;
	}

	if (saveOrderList("Backup1.txt", newOrderList) || saveOrderList("Backup2.txt", newOrderList))
		orderClearedFromStorage = true;
	else {
		saveOrderList("Backup1.txt", orderList);
		saveOrderList("Backup2.txt", orderList);
	}

	if (orderClearedFromList && orderClearedFromStorage)
	{
		orderList = newOrderList;


		if (sendupdate == true){
			std::string msg;
			msg = msgParser_makeClearOrderMsg(order, newOrderList, getMyIP());
			udp_send(msg.c_str(), strlen(msg.c_str()) + 1);
		}

		if ((order.direction == BUTTON_COMMAND && order.assignedIP == getMyIP()) || order.direction != BUTTON_COMMAND)
			ioDriver_clearOrderButtonLamp(order.direction, order.floor);

		transactionSucceeded = true;
	} 
	pthread_mutex_unlock(&orderManagerMutex);
	return transactionSucceeded;
}

bool orderManager_clearOrdersAt(int floor, std::string orderIP, bool sendupdate)
{
	bool transactionSucceeded = false;

	bool ordersClearedFromList = false;
	bool ordersClearedFromStorage = false;	
	OrderList clearedOrders;


	pthread_mutex_lock(&orderManagerMutex);
	OrderList newOrderList = orderList;


	OrderList::iterator search = newOrderList.begin();

	while (search != newOrderList.end()){
		if (search->floor == floor && search->assignedIP == orderIP){
			clearedOrders.push_back(*search);
			search = newOrderList.erase(search);
			ordersClearedFromList = true;
		} else
			++search;
	}	

	if (saveOrderList("Backup1.txt", newOrderList) || saveOrderList("Backup2.txt", newOrderList))
		ordersClearedFromStorage = true;
	else {
		saveOrderList("Backup1.txt", orderList);
		saveOrderList("Backup2.txt", orderList);
	}


	if (ordersClearedFromList && ordersClearedFromStorage)
	{
		orderList = newOrderList;

		if (sendupdate == true){
			std::string msg;
			for (auto it = clearedOrders.begin(); it != clearedOrders.end(); ++it) 
			{
				msg = msgParser_makeClearOrderMsg(*it, newOrderList, orderIP);
				udp_send(msg.c_str(), strlen(msg.c_str()) + 1);
			}
		}

		for (auto it = clearedOrders.begin(); it != clearedOrders.end(); ++it) 
			ioDriver_clearOrderButtonLamp(it->direction, it->floor);
		
		transactionSucceeded = true;
	}

	pthread_mutex_unlock(&orderManagerMutex);
	return transactionSucceeded;
}


bool orderManager_mergeOrderListWith(OrderList orders, bool sendupdate)
{
	bool wasListUpdated = false;
	bool wasStorageUpdated = false;
	bool transactionSucceeded = false;
		
	pthread_mutex_lock(&orderManagerMutex);

	OrderList newOrderList = orderList;

	for (auto it = orders.begin(); it != orders.end(); ++it)
	{
		if (std::find(newOrderList.begin(), newOrderList.end(), (*it)) == newOrderList.end())
		{
			it->timeAssigned = time(0);
			newOrderList.push_back((*it));
			wasListUpdated = true;
		if ((it->direction == BUTTON_COMMAND && it->assignedIP == getMyIP()) || (it->direction != BUTTON_COMMAND))
			ioDriver_setOrderButtonLamp(it->direction, it->floor);
		}
	}

	if (saveOrderList("Backup1.txt", orderList) || saveOrderList("Backup2.txt", orderList) )
		wasStorageUpdated = true;

	if (wasListUpdated && wasStorageUpdated){
		transactionSucceeded = true;

		orderList = newOrderList;

		if (sendupdate == true){
			std::string msg;
			msg = msgParser_makeOrderListMsg(orderList, getMyIP());
			udp_send(msg.c_str(), strlen(msg.c_str()) + 1);
		}
		
	}

	pthread_mutex_unlock(&orderManagerMutex);	
	return transactionSucceeded;
}




bool orderManager_shouldElevatorStopHere(int floor, motor_direction_t direction) 
{
	bool hasOrderHere = false;
	bool hasOrderHereInSameDirection = false;
	bool hasMoreOrdersInSameDirection = false;

	pthread_mutex_lock(&orderManagerMutex);

	for (auto orderPtr = orderList.begin(); orderPtr != orderList.end(); ++orderPtr){
		
		if (orderPtr->floor == floor){
			hasOrderHere = true;
			
			if (orderPtr->direction == BUTTON_COMMAND)
				hasOrderHereInSameDirection = true;

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

	pthread_mutex_unlock(&orderManagerMutex);
	 	
	if (hasOrderHereInSameDirection)
		return true;
	else if (hasOrderHere && !hasMoreOrdersInSameDirection)
		return true;
	else
		return false;
}



motor_direction_t orderManager_getNextMotorDirection(int floor, motor_direction_t lastDirection)
{
	pthread_mutex_lock(&orderManagerMutex);

	OrderList myPendingOrders;
	for (auto orderPtr = orderList.begin(); orderPtr != orderList.end(); ++orderPtr)
		if (orderPtr->assignedIP == getMyIP())
			myPendingOrders.push_back(*orderPtr);

	pthread_mutex_unlock(&orderManagerMutex);

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

int orderManager_getOrderCostSimple(int orderFloor, button_type_t orderButton, int lastFloor, motor_direction_t lastDirection)
{
	int cost = 0;
		
	if (orderFloor == lastFloor)
		return 0;
	
	cost += abs(lastFloor - orderFloor);
	
	motor_direction_t orderDirection;
	
	if (orderButton == BUTTON_CALL_UP)			orderDirection = DIRECTION_UP;
	else if (orderButton == BUTTON_CALL_DOWN)	orderDirection = DIRECTION_DOWN;
	else										orderDirection = DIRECTION_STOP;

	if (((lastDirection == DIRECTION_UP) && (orderFloor < lastFloor)) || ((lastDirection == DIRECTION_DOWN) && (orderFloor > lastFloor)))
	{
		cost += FLOORCOUNT * 2;
		if (orderDirection == lastDirection)
			cost += FLOORCOUNT;
	}
	else
	{
		if (lastDirection != orderDirection)
			cost += FLOORCOUNT;
	}
	return cost;
}





int orderManager_getOrderCost(int orderFloor, button_type_t orderButton, int lastFloor, motor_direction_t lastDirection)
{
	int cost = 0;

	cost += orderManager_getOrderCostSimple(orderFloor, orderButton, lastFloor, lastDirection);

	pthread_mutex_lock(&orderManagerMutex);

	if (cost != 0){
		// A busy elevator will have a higher cost
		for (auto it = orderList.begin(); it != orderList.end(); ++it)
		{
			if (it->assignedIP == getMyIP())
				cost += 3 * orderManager_getOrderCostSimple(it->floor, it->direction, lastFloor, lastDirection);
		}
	}

	pthread_mutex_unlock(&orderManagerMutex);

	return cost;
}





Order orderManager_checkForOrderTimeout()
{
	pthread_mutex_lock(&orderManagerMutex);
	Order order;
	for (auto it = orderList.begin(); it != orderList.end(); ++it)
	{ 
		if (it->timeAssigned + ORDER_TIMEOUT < time(0)) 
			order = *it;
	}

	pthread_mutex_unlock(&orderManagerMutex);
	return order;
}
