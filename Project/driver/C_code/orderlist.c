#include "orderlist.h"

// --- ORDER_EQUALS --- //
int order_equals(order rhs, order lhs)
{
	if ((rhs.floor == lhs.floor) && (rhs.direction == lhs.direction) && (rhs.elevator == lhs.elevator)) return 1;
	else return 0;
}

// --- ORDERLIST_INSERT --- //
int orderlist_insert(order *order, orderlist &list)
{
	if ((list.first != NULL) && (list.last != NULL))
	{
		list.last->next = order;
	}
	else
	{
		order->next = NULL;
		list.first = order;
	}
	list.last = order;
}

// --- ORDERLIST_REMOVE --- //
int orderlist_remove(order order, orderlist &list)
{
	order* current = list.start;
	order* prev = NULL;
	while (current != NULL)
	{
		if (order_equals((*current), order))
		{
			
		}
	}
}
