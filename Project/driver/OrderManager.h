#ifndef ORDERMANAGERNEW_H
#define ORDERMANAGERNEW_H

/* Include all files with the needed datastructures */

enum order_direction_t {
	ORDER_UP = 0,
	ORDER_INSIDE,
	ORDER_DOWN
};

struct Order {
	order_direction_t direction;
	int floor;
	int assignedIP; // Should be string? Or?
	int timeAssigned;
}

class OrderManager {
private:
	OrderList orderList;
public:
	void newOrder(int floor, order_direction_t direction);
	void clearOrder(Order order);
	bool hasOrderOnFloor(int floor);
	motor_direction_t getNextDirection(int floor, motor_direction_t lastDirection);
	int getCost(int floor);
	int mergeOrdersWith(OrderList orders)
};

#endif