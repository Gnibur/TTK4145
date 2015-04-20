#ifndef ORDERMANAGERNEW_H
#define ORDERMANAGERNEW_H

#define IP 4
#define N_FLOORS 4

#include <vector>
#include <string>
#include "DataStructures.h"

struct Order;

typedef std::vector<Order> OrderList;

class OrderManager {
private:
	OrderList orderList;
public:
    // Pushing and popping from the orderlist
	void newOrder(Order order);
	void clearOrder(Order order);
	void mergeMyOrdersWith(OrderList orders);

	// Pulling from orderlist
	OrderList getOrders() { return orderList; }
		
	// Logic and algorithms
	OrderList			getOrdersOnFloorInDirection(int floor, button_type_t direction);
	motor_direction_t	getNextDirection(int floor, motor_direction_t lastDirection);
	int					getCost(int lastFloor, int newFloor, motor_direction_t lastDirection, button_type_t wantedDirection);
};

#endif
