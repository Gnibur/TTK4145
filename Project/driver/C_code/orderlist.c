#include "orderlist.h"

int order_equals(order rhs, order lhs) {
    return ((rhs.floor = lhs.floor) && (rhs.direction == lhs.direction) && (rhs.elevator == lhs.elevator))? 1 : 0;
}

orderlist orderlist_insert(order my_order, orderlist my_list) {
    my_list.list[my_list.elements] = my_order;
    my_list.elements += 1;
    return my_list;
}

orderlist orderlist_remove(order my_order, orderlist my_list) {
    int i;
    for (i = 0; i < my_list.elements; i++)
    {
        if (order_equals(my_order, my_list.list[i]))
        {
            my_list.list[i] = my_list.list[((my_list.elements)-1)];
            my_list.elements -= 1;
            break;
        }
    }
    return my_list;
} 


orderlist substract(orderlist lhs, orderlist rhs) {
    int i;
    int max = rhs.elements;
    for (i = 0; i < max; i++) {
        lhs = orderlist_remove(rhs.list[i], lhs);
    }
    return lhs;
}

void main ()
{
    orderlist my_list;
    my_list.elements = 0;
    order order1;
    order order2;
    
    order1.floor = 1;
    order1.direction = DIRECTION_UP;
    order1.elevator = 1;
    
    order2.floor = 2;
    order2.direction = DIRECTION_UP;
    order2.elevator = 3;
    
    my_list = orderlist_insert(order1, my_list);
    my_list = orderlist_insert(order2, my_list);
    //my_list = orderlist_remove(order1, my_list);
    
    printf("%d\n", my_list.elements);
    
}
