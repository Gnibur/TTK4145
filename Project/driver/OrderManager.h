#ifndef ORDERMANAGERNEW_H
#define ORDERMANAGERNEW_H

#define N_FLOORS 4
#define ORDER_TIMEOUT 50

#include <vector>
#include <string>
#include "DataStructures.h"

#define SEND_UPDATE true
#define DONT_SEND_UPDATE false

void orderManager_recover();


bool orderManager_addOrder(Order order, bool sendupdate);
bool orderManager_clearOrder(Order order, bool sendupdate);
bool orderManager_clearOrdersAt(int floor, std::string orderIP, bool sendupdate);
void orderManager_mergeOrderListWith(OrderList orders, bool sendupdate);

Order orderManager_checkForOrderTimeout();

bool 				orderManager_shouldStopHere(int floor, motor_direction_t direction); 
motor_direction_t	orderManager_getNextDirection(int floor, motor_direction_t lastDirection);
int					orderManager_getCost(int lastFloor, int newFloor, motor_direction_t lastDirection, button_type_t wantedDirection);

#endif
