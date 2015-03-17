#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <vector>
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
    void mergeList();
};

#endif
