#ifndef ORDERMANAGERNEW_H
#define ORDERMANAGERNEW_H

#define IP 4
#define N_FLOORS 4

#include <vector>
#include "DataStructures.h"

struct Order;

typedef std::vector<Order> OrderList;

class OrderManager {
private:
	OrderList orderList;
public:
	void newOrder(int floor, order_direction_t direction);
	void clearOrder(Order order);
	void clearOrders(OrderList orders);
	void mergeMyOrdersWith(OrderList orders);
	OrderList getOrders() { return orderList; }
		
	OrderList findOrdersOnFloorInDirection(int floor, motor_direction_t direction);
	bool hasOrderOnFloor(int floor);
	motor_direction_t getNextDirection(int floor, motor_direction_t lastDirection);
	int getCost(int lastFloor, int newFloor, motor_direction_t lastDirection, order_direction_t wantedDirection); // This _might_ be confusing
};

#endif
