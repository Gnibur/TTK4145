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
}

motor_direction_t OrderManager::getNextDirection(int floor, motor_direction_t lastDirection)
{

}

int OrderManager::getCost(int floor)
{

}

int mergeOrdersWith(OrderList orders)
{

}