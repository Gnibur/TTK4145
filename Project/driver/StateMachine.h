#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "OrderManager.h"
#include "DataStructures.h"
//#include "IoDriver.h"

#define BROADCAST_PORT 16
#define MAXLENGTH_BUF 2048

void eventButtonPressed(int floor, button_type_t button);
void eventFloorReached(int floor, motor_direction_t direction);
void eventDoorTimeout();
void orderTimeOut(Order order);

#endif
