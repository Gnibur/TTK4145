#ifndef ORDERMANAGERNEW_H
#define ORDERMANAGERNEW_H

#define N_FLOORS 4
#define ORDER_TIMEOUT 50

#include <vector>
#include <string>
#include "DataStructures.h"


void orderManager_init();

// Pushing and popping from the orderlist
bool 				orderManager_addOrder(Order order);
bool 				orderManager_clearOrder(Order order);
void				orderManager_mergeMyOrdersWith(OrderList orders);
OrderList 			orderManager_getOrders();
		
// Logic and algorithms
OrderList			orderManager_getOrdersOnFloor(int floor);
bool 				orderManager_shouldStopHere(int floor, motor_direction_t direction); 
motor_direction_t	orderManager_getNextDirection(int floor, motor_direction_t lastDirection);
int					orderManager_getCost(int lastFloor, int newFloor, motor_direction_t lastDirection, button_type_t wantedDirection);
bool				orderManager_orderListEquals(OrderList rhs);
Order				orderManager_checkForOrderTimeout();

#endif
