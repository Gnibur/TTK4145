#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <vector>
#include <algorithm>
//#include "IoDriver.h"

typedef enum {
    DIRECTION_DOWN,
    DIRECTION_UNDEFINED,
    DIRECTION_UP
} order_direction_t;

struct Order {
    int floor;
    order_direction_t direction;
    int elevator;

	// Overload Operators
    bool operator==(const Order& rhs) const
	{
	    return (floor == rhs.floor && direction == rhs.direction && elevator == rhs.elevator);
	}
	bool operator < (const Order& rhs) const
	{
		if (floor < rhs.floor) 			return true;
		if (floor > rhs.floor) 			return false;
		if (elevator < rhs.floor) 		return true;
		if (elevator > rhs.floor) 		return false;
		if (direction < rhs.direction) 	return true;
		return false;
	}
};

typedef std::vector<Order> OrderList;

class OrderManager {
private:
	OrderList orderList;
	int myID;
	//OrderList stashedList;
public:
    OrderManager();
    int getFloorWithLowestCost(int currentFloor, order_direction_t currentDirection);
    int mergeList(OrderList list);
    int updateList(int status, OrderList list, Order order);
    OrderList getList() {return orderList; }


    // TEST FUNCTIONS START
    void addOrder(Order order) {orderList.push_back(order); }
    void clear() {orderList.clear(); }
    void printOrders ();
    void sortOrders() {std::sort(orderList.begin(), orderList.end()); }
	// TEST FUNCTIONS END
};

#endif
