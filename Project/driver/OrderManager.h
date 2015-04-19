#ifndef ORDERMANAGERNEW_H
#define ORDERMANAGERNEW_H

#define IP 4
#define N_FLOORS 4

#include <vector>
#include "DataStructures.h"

struct Order;

/* Include all files with the needed datastructures */


// Maybe this should be button_type_t? Then we need to include "IoDriver.h"

typedef std::vector<Order> OrderList;

class OrderManager {
private:
	OrderList orderList;
	OrderList globalList;
public:
	void newOrder(int floor, order_direction_t direction);
	void clearOrder(Order order);
	void clearOrdersOnFloor(int floor, motor_direction_t direction);
	bool hasOrderOnFloor(int floor);
	motor_direction_t getNextDirection(int floor, motor_direction_t lastDirection);
	// NB! getCost _might_ be a bit too confusing. I should get another set of eyes on it.
	int getCost(int lastFloor, int newFloor, motor_direction_t lastDirection, order_direction_t wantedDirection);
	void mergeMyOrdersWith(OrderList orders);	
	OrderList getGlobalOrders() { return globalList; }
};

#endif
