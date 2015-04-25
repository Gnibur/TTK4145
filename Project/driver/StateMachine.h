#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "DataStructures.h"

void stateMachine_initialize();
void stateMachine_eventButtonPressed(int floor, button_type_t button);
void stateMachine_eventFloorReached(int floor);
void stateMachine_eventFloorLeft();
void stateMachine_eventNewOrderArrived(Order order);

void stateMachine_eventDoorTimedOut();
void stateMachine_eventOrderTimedOut(Order order);


int 			getLastFloor();
motor_direction_t 	getLastDirection();

#endif
