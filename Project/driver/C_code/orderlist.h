#ifndef ORDERLIST_H
#define ORDERLIST_H

#include <stdio.h>

typedef enum {
    DIRECTION_DOWN,
    DIRECTION_UNDEFINED,
    DIRECTION_UP
} order_direction_t;


typedef struct Order {
    int floor;
    order_direction_t direction;
    int elevator;
} order;

typedef struct Orderlist {
	struct Order list[1024];
	int elements;
} orderlist;

int order_equals(order rhs, order lhs);
orderlist orderlist_insert(order my_order, orderlist my_list); 	//Return -1 if not succesfully inserted
orderlist orderlist_remove(order my_order, orderlist my_list); 	//Return -1 if not succesfully removed
orderlist substract(orderlist lhs, orderlist rhs);				//Returns all orders in lhs that are not in rhs
#endif
