#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "OrderManager.h"
#include "DataStructures.h"
#include "Timer.h"
#include "IoDriver.h"
#include "udp.h"


void stateMachine_initialize();
void stateMachine_buttonPressed(int floor, button_type_t button);
void stateMachine_floorReached(int floor);
bool stateMachine_shouldIStopHere(OrderList ordersOnFloor);
void stateMachine_doorTimeout();
void stateMachine_orderTimeOut(Order order);

void stateMachine_newOrder(int floor, button_type_t button);
void stateMachine_updateDirection();

int 			getLastFloor();
motor_direction_t 	getLastDirection();

#endif
