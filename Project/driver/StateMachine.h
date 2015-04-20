#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "OrderManager.h"
#include "DataStructures.h"
//#include "IoDriver.h"

#define BROADCAST_PORT 16
#define MAXLENGTH_BUF 2048

void stateMachine_buttonPressed(int floor, button_type_t button);
void stateMachine_floorReached(int floor, motor_direction_t direction);
void stateMachine_doorTimeout();
void stateMachine_orderTimeOut(Order order);

#endif
