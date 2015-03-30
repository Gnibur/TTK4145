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
    order* next;
} order;

typedef struct Orderlist {
	order* first;
	order* last;
} orderlist;

int order_equals(order rhs, order lhs);
int orderlist_insert(order *order, orderlist &list); //Return -1 if not succesfully inserted
int orderlist_remove(order *order, orderlist &list); //Return -1 if not succesfully removed

#endif
