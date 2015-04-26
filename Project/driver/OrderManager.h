#ifndef ORDERMANAGERNEW_H
#define ORDERMANAGERNEW_H

#define N_FLOORS 4
#define ORDER_TIMEOUT 50

#include <vector>
#include <string>
#include "DataStructures.h"

#define SEND_UPDATE true
#define DONT_SEND_UPDATE false

void orderManager_recoverFromBackup();


bool orderManager_addOrder(Order order, bool sendupdate);
bool orderManager_clearOrder(Order order, bool sendupdate);
bool orderManager_clearOrdersAt(int floor, std::string orderIP, bool sendupdate);
void orderManager_mergeOrderListWith(OrderList orders, bool sendupdate);

Order orderManager_checkForOrderTimeout();

bool 				orderManager_shouldElevatorStopHere(int floor, motor_direction_t direction); 
motor_direction_t	orderManager_getNextMotorDirection(int floor, motor_direction_t lastDirection);
int					orderManager_getOrderCost(int orderFloor, button_type_t orderDirection, int lastFloor, motor_direction_t lastDirection);

#endif
