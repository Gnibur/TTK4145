#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "OrderManager.h"
#include "DataStructures.h"
#include "Timer.h"
#include "IoDriver.h"
#include "udp.h"

#define BROADCAST_PORT 16
#define MAXLENGTH_BUF 2048

void stateMachine_initialize();
void stateMachine_buttonPressed(int floor, button_type_t button);
void stateMachine_floorReached(int floor);
void stateMachine_doorTimeout();
void stateMachine_orderTimeOut(Order order);

int 				getLastFloor();
motor_direction_t 	getLastDirection();

#endif
